#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include "glad/glad.h"

#include <cstdint>
#include "cbpp/vec2.h"

#include <map>
#include <stdio.h>
#include <stddef.h>

#define CBVS_STRICT 1
#define CBVS_TOLERANT 0

#define CBVS_SHADERS_STRICT CBVS_STRICT

namespace cbvs {
    using cbpp::float_t;

	extern const char *default_vtx, *default_frag;

	class Shader {
        public:
            Shader() = default;
            Shader(GLenum type, const char* source);
            Shader(const char* shname, GLenum type, const char* source);
            Shader(const Shader& other) = delete;

            void SetName(const char* name);
            const char* Name();

            bool Compile(GLenum type, const char* src);
			GLuint ID();

            bool IsValid();

            ~Shader();

        private:
            char name[64];
            bool compiled = false;
            GLuint objid;
    };

    struct PipeData {
        Shader *vtx, *frag, *geom;
    };

    class Pipe {
        public:
            Pipe() = default;
            Pipe(Shader* vtxp, Shader* fragp);
            Pipe(Shader* vtxp, Shader* fragp, Shader* geop);
            Pipe(PipeData& data);
            Pipe(const Pipe& other) = delete;

            bool Link(PipeData& sharr);
            GLuint ID();

            bool IsValid();
            void Use();

            GLint GetUniform(const char* uname);
            bool HasUniform(const char* uname);

            void PushUniform(const char* name, float_t a, float_t b, float_t c, float_t d);
            void PushUniform(const char* name, float_t a);

            void PushUniform(const char* name, int32_t a);

            ~Pipe();

        private:
            void Create();
            bool linked = false;
            GLuint objid;
    };

    extern Shader *DefaultVTX, *DefaultFRAG;
    extern Pipe* DefaultPipe;

    bool InitDefaultShaders();
    void CleanupDefaultShaders();

    Shader* CreateShader(GLenum type, const char* src);

    const char* LoadShader(const char* path, GLenum type = GL_VERTEX_SHADER);

    Pipe* CreatePipe(PipeData& data);
    Pipe* CreatePipe(Shader* vtxp, Shader* fragp);
    Pipe* CreatePipe(Shader* vtxp, Shader* fragp, Shader* geop);
}

#endif
