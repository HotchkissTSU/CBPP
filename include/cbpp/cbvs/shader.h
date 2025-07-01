#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include <map>
#include <stdint.h>

#include "glad/glad.h"
#include "cbpp/cbstring.h"
#include "cbpp/print.h"
#include "cbpp/vec2.h"
#include "cbpp/color.h"
#include "cbpp/cbstring.h"

#define CBVS_SHADER_VERSION ("#version 400 core")

#define CBVS_SHADER_REGISTER(sName, sVTX, sFRAG)\
    cbvs::__shloader_insert( new cbvs::ShaderLoaderNode(sName, sVTX, sFRAG, NULL) );

#define CBVS_SHADER_REGISTER_EX(sName, sVTX, sFRAG, sGEOM)\
    cbvs::__shloader_insert( new cbvs::ShaderLoaderNode(sName, sVTX, sFRAG, sGEOM) );

namespace cbvs {
    /*
        All shaders are loaded in 2 steps: 
        1) We form a list of all the pipelines we want to compile, link and mount
        2) We sort out repeating shaders not to do extra work and then perform all of the OGL`s magic
    */
    struct ShaderLoaderNode final : public cbpp::BasePrintable {
        ShaderLoaderNode(const char* sName, const char* sVTX, const char* sFRAG, const char* sGEOM = NULL);

        //This thing is also printable for debugging purposes
        virtual void Print(FILE* hStream = stdout) const;
        virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const;

        ShaderLoaderNode* m_pNextNode = NULL;
        
        char *m_sVTX, *m_sFRAG, *m_sGEOM;
        char* m_sPipeName;

        bool m_bStrict = true;

        ~ShaderLoaderNode();
    };

    extern ShaderLoaderNode* g_pShadersHead;

    //Insert a new raw pipeline data in the loading list
    void __shloader_insert(ShaderLoaderNode* pNext) noexcept;

    //Attempt to load all registered shaders
    bool LoadShaders();
    void CleanupShaders() noexcept;
}

namespace cbvs {
    struct GLSL_CommInfo {
        int iCommand;
        char* sArgument;
        size_t iFullLength;
    };

    //Obtain a file extension for different shader types` sources
    const char* GetShaderFileExtension(GLenum iShaderClass) noexcept;

    GLSL_CommInfo* LocatePrepCommands(const char* sSource, size_t* piAmount) noexcept;

    //Add support for #include directives in GLSL
    char** PreprocessShaderSource(char* sSource, size_t** ppLengths) noexcept;

    //Load and compile a single shader of the given type
    GLuint CreateShader(const char* sPath, GLenum iShaderClass) noexcept;

    //A full rendering pipeline
    class Pipe {
        public:
            Pipe(const char* sName) : m_sName(sName) {};

            const char* Name() const noexcept;

            bool Load(GLuint hVTX, GLuint hGEOM, GLuint hFRAG) noexcept;

            //bool Load(const char* sVTX, const char* sFRAG, const char* sGEOM = NULL) noexcept;

            void Use() const noexcept;

            GLint GetHandle() const noexcept;

            GLint GetUniform(const char* sName) const noexcept;

            bool PushUniform(const char* sName, cbpp::float_t fValue) noexcept;
            bool PushUniform(const char* sName, int32_t iValue) noexcept;

            bool PushUniform(const char* sName, cbpp::Vec2 v1) noexcept;
            bool PushUniform(const char* sName, cbpp::Color iColor) noexcept;

            ~Pipe();

        private:
            cbpp::CString m_sName;
            GLint m_hPipeID;
    };

    Pipe* GetPipe(const char* sPipeName) noexcept;

    extern std::map<cbpp::CString, Pipe*> g_mShaderDict;
}

#endif
