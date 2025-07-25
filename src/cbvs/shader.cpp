#include "cbvs/shader.h"

#include "cbpp/error.h"
#include "cbpp/fileio.h"
#include "cbpp/cb_alloc.h"

namespace cbvs {
    ShaderLoaderNode* g_pShadersHead = NULL;

    ShaderLoaderNode::ShaderLoaderNode(const char* sName, const char* sVTX, const char* sFRAG, const char* sGEOM) {
        m_sVTX = strdup(sVTX);
        m_sFRAG = strdup(sFRAG);
        if(sGEOM != NULL) {
            m_sGEOM = strdup(sGEOM);
        }else{
            m_sGEOM = NULL;
        }

        m_sPipeName = strdup(sName);
    }

    ShaderLoaderNode::~ShaderLoaderNode() {
        free(m_sFRAG);
        free(m_sVTX);
        if(m_sGEOM != NULL) {
            free(m_sGEOM);
        }

        free(m_sPipeName);
    }

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
    std::map<cbpp::CString, Pipe*> g_mShaderDict;

    const char* GetShaderFileExtension(GLenum iShaderClass) noexcept {
        switch (iShaderClass) {
            case GL_VERTEX_SHADER:   return ".vtx";  break;
            case GL_FRAGMENT_SHADER: return ".frag"; break;
            case GL_GEOMETRY_SHADER: return ".geom"; break;
            default: return "";
        }
    }

    enum {
        GLSL_INCLUDE,       // C-like includes
        GLSL_AUTOVERSION    // Automatically insert a "#version ... core" string
    };

    inline bool GLSL_IsSpace(char cSource, bool bQuoted) {
        if(bQuoted) { return false; }
        return (cSource == '\n') || (cSource == ' ') || (cSource == '\t');
    }

    GLSL_CommInfo* LocatePrepCommands(const char* sSource, size_t* piAmount) noexcept {
        char* pCurrent = (char*) sSource;
        (*piAmount) = 0;

        char sCommand[128]; size_t iCmd = 0;
        char sArgument[256]; size_t iArg = 0;

        int iLineCounter = 0;

        GLSL_CommInfo* pOut = NULL;
        bool bQuoted = false;

        while(*pCurrent) {
            iLineCounter += (*pCurrent == '\n');

            if(*pCurrent == '#') {
                iCmd = 0; iArg = 0;

                // Read the command
                while(!GLSL_IsSpace(*pCurrent, false) && (*pCurrent)) {
                    if(iCmd < 127) {
                        sCommand[iCmd] = *pCurrent;
                        sCommand[iCmd+1] = '\0';
                    }

                    iCmd++;
                    pCurrent++;
                }

                pCurrent++;

                // Read the argument
                while(1) {
                    if(*pCurrent == '"') { bQuoted = !bQuoted; }

                    if( (!bQuoted && GLSL_IsSpace(*pCurrent, bQuoted)) || !(*pCurrent) ) {
                        break;
                    }

                    if(iArg < 255) {
                        sArgument[iArg] = *pCurrent;
                        sArgument[iArg+1] = '\0';
                    }

                    iArg++;
                    pCurrent++;
                }

                // Perform some sanity checks

                if( iCmd != 0 && iArg == 0 ) {
                    snprintf(sArgument, 256, "(%u) Command '%s' does not have an argument", iLineCounter, sCommand);
                    cbpp::PushError(cbpp::ERROR_IO, sArgument);

                    cbpp::Free(pOut);
                    return NULL;
                }

                // Empty (how?), or only '#' alone
                if( iCmd < 2 ) {
                    snprintf(sArgument, 256, "(%u) Empty preprocessor command", iLineCounter);
                    cbpp::PushError(cbpp::ERROR_IO, sArgument);

                    cbpp::Free(pOut);
                    return NULL;
                }

                (*piAmount)++;
                pOut = cbpp::Realloc<GLSL_CommInfo>(pOut, *piAmount);
                GLSL_CommInfo* pThis = &pOut[ (*piAmount) - 1 ];

                if( strcmp(sCommand, "#include") == 0 ) {
                    pThis->iCommand = GLSL_INCLUDE;
                }else if( strcmp(sCommand, "#autoversion") == 0 ) {
                    pThis->iCommand = GLSL_AUTOVERSION;
                }

                if(iArg > 0) {
                    pThis->sArgument = strdup(sArgument);
                }else{
                    pThis->sArgument = NULL;
                }

                pThis->iFullLength = iArg + iCmd + 1;
            }

            pCurrent++;
        }

        return pOut;
    }
    
    inline void GLSL_PushSource(char** sBlocks, char* sText, size_t* piLength) noexcept {
        (*piLength)++;
        sBlocks = cbpp::Realloc<char*>(sBlocks, *piLength);
    }

    char** PreprocessShaderSource(const char* sSource, size_t** ppLengths) noexcept {
        size_t iCommsAmount;
        GLSL_CommInfo* pCommsInfo = LocatePrepCommands(sSource, &iCommsAmount);

        if(iCommsAmount == 0) {
            return NULL;
        }

        char** pOut = NULL;

        char *pPrev = (char*)(sSource), *pCur;
        for(size_t i = 0; i < iCommsAmount; i++) {
            GLSL_CommInfo& Cur = pCommsInfo[i];

            
        }
    }

    GLuint CreateShader(const char* sPath, GLenum iShaderType) noexcept {
        char sShaderNameBuffer[128];
        snprintf(sShaderNameBuffer, 128, "%s%s", sPath, GetShaderFileExtension(iShaderType));

        cbpp::File* hFile = cbpp::OpenFile(cbpp::PATH_SHADER, sShaderNameBuffer, "rt");
        if(hFile == NULL) { return -1; }

        size_t iFileLength = hFile->Length();

        char* sFileText = cbpp::Malloc<char>(iFileLength+1);
        sFileText[iFileLength] = '\0';

        hFile->Read(sFileText, hFile->Length());
        delete hFile;

        GLuint hShader = glCreateShader(iShaderType);
        glShaderSource(hShader, 1, &sFileText, NULL);
        glCompileShader(hShader);

        cbpp::Free(sFileText);

        GLint iResult;
        glGetShaderiv(hShader, GL_COMPILE_STATUS, &iResult);
        if(iResult == GL_FALSE) {
            char sErrorLog [512], sShaderLog[512];

            GLint iSize = 0;
            glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &iSize);

            glGetShaderInfoLog(hShader, std::min(iSize, 512), NULL, sShaderLog);

            snprintf(sErrorLog, 512, "Shader '%s':\n%s", sShaderNameBuffer, sShaderLog);

            cbpp::PushError(cbpp::ERROR_GL, sErrorLog);
            
            return -1;
        }

        return hShader;
    }

    bool Pipe::Load(GLuint hVTX, GLuint hGEOM, GLuint hFRAG) noexcept {
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

        glDetachShader(m_hPipeID, hVTX);
        glDetachShader(m_hPipeID, hFRAG);
        if(hGEOM != -1) {
            glDetachShader(m_hPipeID, hGEOM);
        }

        return true;
    }
    
    GLint Pipe::GetUniform(const char* sName) const noexcept {
        static int s_iUniformErrorCounter = 0;

        GLint hOut = glGetUniformLocation(m_hPipeID, sName);

        if(hOut == -1) {
            if(s_iUniformErrorCounter < 16) {
                char sBuffer[128];
                snprintf(sBuffer, 128, "Failed to get '%s' uniform location from the pipe '%s'", sName, m_sName);
                cbpp::PushError(cbpp::ERROR_GL, sBuffer);
            }

            s_iUniformErrorCounter++;
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

    Pipe::~Pipe() {
        glDeleteProgram(m_hPipeID);
    }

    bool Pipe::PushUniform(const char* sName, cbpp::float_t fValue) noexcept {
        GLint hLocation = this->GetUniform(sName);
        if(hLocation != -1) {
            glUniform1f(hLocation, (GLfloat)fValue);
            return true;
        }
        return false;
    }

    bool Pipe::PushUniform(const char* sName, cbpp::Vec2 vValue) noexcept {
        GLint hLocation = this->GetUniform(sName);
        if(hLocation != -1) {
            glUniform2f(hLocation, (GLfloat)vValue.x, (GLfloat)vValue.y);
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
            char sBuffer[128]; snprintf(sBuffer, 128, "Failed to find a pipeline named '%s'", sName);
            cbpp::PushError(cbpp::ERROR_GL, sBuffer);
            return NULL;
        }

        return g_mShaderDict.at(hName);
    }

    bool LoadShaders() {
        FILE* hLogStream = stdout;

        FILE* hLog = fopen("logs/shader.txt", "wt");
        if(hLog == NULL) {
            CbThrowWarning("Unable to open shader.txt log file");
        }else{
            hLogStream = hLog;
        }

        ShaderLoaderNode* pCurrent = g_pShadersHead, *pPrev = NULL;

        std::map<cbpp::CString, GLuint> mShadersVTX, mShadersFRAG, mShadersGEOM;

        while(pCurrent != NULL) {
            /*#ifdef CBPP_DEBUG
                cbpp::Print(*pCurrent);
            #endif*/

            GLuint hVTX, hGEOM = -1, hFRAG;
            
            if(mShadersVTX.count(pCurrent->m_sVTX) == 0) {
                hVTX = CreateShader(pCurrent->m_sVTX, GL_VERTEX_SHADER);
                if(hVTX == -1 && pCurrent->m_bStrict) { return false; }
                mShadersVTX[pCurrent->m_sVTX] = hVTX;
            }else{
                hVTX = mShadersVTX[pCurrent->m_sVTX];
            }

            if(mShadersFRAG.count(pCurrent->m_sFRAG) == 0) {
                hFRAG = CreateShader(pCurrent->m_sFRAG, GL_FRAGMENT_SHADER);
                if(hFRAG == -1 && pCurrent->m_bStrict) { return false; }
                mShadersFRAG[pCurrent->m_sFRAG] = hFRAG;
            }else{
                hFRAG = mShadersFRAG[pCurrent->m_sFRAG];
            }

            if(pCurrent->m_sGEOM != NULL) {
                if(mShadersGEOM.count(pCurrent->m_sGEOM) == 0) {
                    hGEOM = CreateShader(pCurrent->m_sGEOM, GL_GEOMETRY_SHADER);
                    if(hGEOM == -1 && pCurrent->m_bStrict) { return false; }
                    mShadersGEOM[pCurrent->m_sGEOM] = hGEOM;
                }else{
                    hGEOM = mShadersGEOM[pCurrent->m_sGEOM];
                }
            }

            Pipe* pNewPipe = new Pipe(pCurrent->m_sPipeName);
            if(!pNewPipe->Load(hVTX, hGEOM, hFRAG)) {
                delete pNewPipe;
                return false;
            }

            cbpp::CString hName(pCurrent->m_sPipeName);
            
            g_mShaderDict[hName] = pNewPipe;

            pPrev = pCurrent;
            pCurrent = pCurrent->m_pNextNode;

            if(pPrev != NULL) {
                delete pPrev;
            }
        }

        fprintf(hLogStream, "Unique vertex shaders: \n");

        for(auto it = mShadersVTX.begin(); it != mShadersVTX.end(); it++) {
            glDeleteShader(it->second);
            fprintf(hLogStream, "\t%s\n", (const char*)(it->first));
        }

        fprintf(hLogStream, "\nUnique fragment shaders: \n");

        for(auto it = mShadersFRAG.begin(); it != mShadersFRAG.end(); it++) {
            glDeleteShader(it->second);
            fprintf(hLogStream, "\t%s\n", (const char*)(it->first));
        }

        fprintf(hLogStream, "\nUnique geometry shaders: \n");

        for(auto it = mShadersGEOM.begin(); it != mShadersGEOM.end(); it++) {
            glDeleteShader(it->second);
            fprintf(hLogStream, "\t%s\n", (const char*)(it->first));
        }

        fprintf(hLogStream, "\nRegistered pipelines list:\n");

        for(auto it = g_mShaderDict.begin(); it != g_mShaderDict.end(); it++) {
            fprintf(hLogStream, "\t%s\n", (const char*)(it->first));
        }

        if(hLog != NULL) {
            fclose(hLog);
        }

        return true;
    }

    void CleanupShaders() noexcept {
        for(auto it = g_mShaderDict.begin(); it != g_mShaderDict.end(); it++) {
            delete it->second;
        }
    }
}
