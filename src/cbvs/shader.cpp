#include "cbvs/shader.h"

#include "cbpp/error.h"
#include "cbpp/fileio.h"

namespace cbvs {
    ShaderLoaderNode* g_pShadersHead = NULL;

    ShaderLoaderNode::ShaderLoaderNode(const char* sName, const char* sVTX, const char* sFRAG, const char* sGEOM) :
                        m_sPipeName(sName), m_sVTX(sVTX),
                        m_sFRAG(sFRAG), m_sGEOM(sGEOM) {}

    void ShaderLoaderNode::Print(FILE* hStream) const {
        fprintf(
            hStream,
            "Shader loading unit: '%s':\n\tVertex shader: %s\n\tFragment shader: %s\n\tGeometry shader: %s\n",
            m_sPipeName, m_sVTX, m_sFRAG, m_sGEOM
        );
    }

    size_t ShaderLoaderNode::SPrint(char* sTarget, size_t iMaxWrite) const {
        return snprintf(
            sTarget, iMaxWrite,
            "Shader loading unit: '%s':\n\tVertex shader: %s\n\tFragment shader: %s\n\tGeometry shader: %s\n",
            m_sPipeName, m_sVTX, m_sFRAG, m_sGEOM
        );
    }

    void __shloader_insert (ShaderLoaderNode* pNode) noexcept {
        if(g_pShadersHead == NULL) { //first entry in the list
            g_pShadersHead = pNode;
        }else{
            pNode->m_pNextNode = g_pShadersHead;
            g_pShadersHead = pNode;
        }
    }
}

namespace cbvs {
    bool g_bSupressPipeCache = false;
    std::map<cbpp::CString, Pipe*> g_mShaderDict;

    const char* GetShaderFileExtension(GLenum iShaderClass) noexcept {
        switch (iShaderClass) {
            case GL_VERTEX_SHADER:   return ".vtx";  break;
            case GL_FRAGMENT_SHADER: return ".frag"; break;
            case GL_GEOMETRY_SHADER: return ".geom"; break;
            default: return "";
        }
    }
    /*
    void GetCachedPipeName(char* sTarget, const char* sPipeName) noexcept {
        for(size_t i = 0; i < strlen(sPipeName); i++) {
            if(sPipeName[i] == '/' || sPipeName == '\\') {
                sTarget[i] = '.';
            }else{
                sTarget[i] = sPipeName[i];
            }
        }
    }

    bool PipeCached(const char* sPipeName) noexcept {
        char* sCacheName = strdup(sPipeName);
        GetCachedPipeName(sCacheName, sPipeName);

        char sPathBuffer[256];
        snprintf(sPathBuffer, 256, "cache/%s", sCacheName);
        free(sCacheName);

        cbpp::File* hTest = cbpp::OpenFile(cbpp::PATH_SHADER, sPathBuffer, "rb");
        if(hTest != NULL) {
            delete hTest;
            return true;
        }

        return false;
    }
    */
    
    GLuint CreateShader(const char* sPath, GLenum iShaderType) noexcept {
        char sShaderNameBuffer[128];
        snprintf(sShaderNameBuffer, 128, "%s%s", sPath, GetShaderFileExtension(iShaderType));

        cbpp::File* hFile = cbpp::OpenFile(cbpp::PATH_SHADER, sShaderNameBuffer, "rt");
        if(hFile == NULL) { return -1; }

        size_t iFileLength = hFile->Length();

        char* sFileText = (char*) malloc(iFileLength+1);
        sFileText[iFileLength] = '\0';

        hFile->Read(sFileText, hFile->Length());
        delete hFile;

        GLuint hShader = glCreateShader(iShaderType);
        glShaderSource(hShader, 1, &sFileText, NULL);
        glCompileShader(hShader);

        free(sFileText);

        GLint iResult;
        glGetShaderiv(hShader, GL_COMPILE_STATUS, &iResult);
        if(iResult == GL_FALSE) {
            GLint iSize = 0;
            glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &iSize);
            
            char* sErrorLog = (char*) malloc(iSize+1);
            sErrorLog[iSize] = '\0';
            glGetShaderInfoLog(hShader, iSize, NULL, sErrorLog);

            cbpp::PushError(cbpp::ERROR_GL, sErrorLog);
            
            return -1;
        }

        return hShader;
    }

    bool Pipe::Load(const char* sVTX, const char* sFRAG, const char* sGEOM) noexcept {
        GLuint hVTX = CreateShader(sVTX, GL_VERTEX_SHADER);
        if(hVTX == -1) {
            return false;
        }

        GLuint hFRAG = CreateShader(sFRAG, GL_FRAGMENT_SHADER);
        if(hFRAG == -1) {
            glDeleteShader(hVTX);
            return false;
        }

        GLuint hGEOM = -1;
        if(sGEOM != NULL) {
            hGEOM = CreateShader(sGEOM, GL_GEOMETRY_SHADER);
            if(hGEOM == -1) {
                CbThrowWarningf("'%s' geometry shader compilation has failed. Pretending that nothing has happened", sGEOM);
            }
        }

        m_hPipeID = glCreateProgram();
        glAttachShader(m_hPipeID, hVTX);
        glAttachShader(m_hPipeID, hFRAG);
        if(hGEOM != -1) { glAttachShader(m_hPipeID, hGEOM); }

        glLinkProgram(m_hPipeID);

        GLint bLinkResult = false;
        glGetProgramiv(m_hPipeID, GL_LINK_STATUS, &bLinkResult);
        if(!bLinkResult) {
            GLint iLogSize;
            glGetProgramiv(m_hPipeID, GL_INFO_LOG_LENGTH, &iLogSize);

            char* sLog = (char*) malloc(iLogSize+1);
            sLog[iLogSize] = '\0';
            glGetProgramInfoLog(m_hPipeID, iLogSize, NULL, sLog);

            cbpp::PushError(cbpp::ERROR_GL, sLog);

            return false;
        }

        return true;
    }

    GLint Pipe::GetUniform(const char* sName) const noexcept {
        GLint hOut = glGetUniformLocation(m_hPipeID, sName);

        if(hOut == -1) {
            char sBuffer[128];
            snprintf(sBuffer, 128, "Failed to get '%s' uniform location from the pipe '%s'", sName, m_sName);
            cbpp::PushError(cbpp::ERROR_GL, sBuffer);
        }

        return hOut;
    }

    const char* Pipe::Name() const noexcept {
        return m_sName;
    }

    void Pipe::Use() const noexcept {
        glUseProgram(m_hPipeID);
    }

    GLint Pipe::GetHandle() const noexcept {
        return m_hPipeID;
    }

    bool Pipe::PushUniform(const char* sName, cbpp::float_t fValue) noexcept {
        GLint hLocation = this->GetUniform(sName);
        if(hLocation != -1) {
            glUniform1f(hLocation, (GLfloat)fValue);
            return true;
        }
        return false;
    }

    bool Pipe::PushUniform(const char* sName, int32_t iValue) noexcept {
        GLint hLocation = this->GetUniform(sName);
        if(hLocation != -1) {
            glUniform1i(hLocation, iValue);
            return true;
        }
        return false;
    }

    bool Pipe::PushUniform(const char* sName, cbpp::Color iColor) noexcept {
        GLint hLocation = this->GetUniform(sName);
        if(hLocation != -1) {
            cbpp::NormColor cNorm = iColor.Normalized();

            glUniform4f(hLocation, cNorm.r, cNorm.g, cNorm.b, cNorm.a);
            return true;
        }
        return false;
    }

    Pipe* GetPipe(const char* sName) noexcept {
        cbpp::CString hName(sName);
        if(g_mShaderDict.count(hName) == 0) {
            return NULL;
        }

        return g_mShaderDict.at(hName);
    }

    bool LoadShaders() {
        ShaderLoaderNode* pCurrent = g_pShadersHead;

        while(pCurrent != NULL) {
            #ifdef CBPP_DEBUG
                cbpp::Print(*pCurrent);
            #endif

            Pipe* pNewPipe = new Pipe(pCurrent->m_sPipeName);
            if(!pNewPipe->Load(pCurrent->m_sVTX, pCurrent->m_sFRAG, pCurrent->m_sGEOM)) {
                delete pNewPipe;
                return false;
            }

            cbpp::CString hName(pCurrent->m_sPipeName);
            g_mShaderDict[hName] = pNewPipe;

            pCurrent = pCurrent->m_pNextNode;
        }

        return true;
    }

    void CleanupShaders() noexcept {
        
    }
}

