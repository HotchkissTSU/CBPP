#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include "glad/glad.h"

#include <cstdint>
#include "cbpp/cbstring.h"

#include <map>
#include <stdio.h>
#include <stddef.h>

#define CBVS_STRICT 1
#define CBVS_TOLERANT 0

#define CBVS_SHADERS_STRICT CBVS_STRICT

namespace cbvs {
	extern const char *default_vtx, *default_frag;

	class Shader {
        public:
            Shader() = default;
            Shader(GLenum type, const char* source);
            Shader(const Shader& other) = delete;

            bool Compile(GLenum type, const char* src);
			GLuint ID();

            bool IsValid();

            ~Shader();

        private:
            bool compiled = false;
            GLuint objid;
    };

    class Pipe {
        public:
            Pipe() = default;
            Pipe(Shader* vtxp, Shader* fragp);
            Pipe(Shader* vtxp, Shader* fragp, Shader* geop);
            Pipe(Shader** sharr, size_t ln );
            Pipe(const Pipe& other) = delete;

            bool Link(Shader** sharr, size_t ln);
            const GLuint* GetShaders();
            GLuint ID();

            bool IsValid();
            void Use();

            ~Pipe();

        private:
            void Create();
            bool linked = false;
            GLuint objid;
    };

    Shader* CreateShader(GLenum type, const char* src);

    Pipe* CreatePipe(Shader** sharr, size_t ln);
    Pipe* CreatePipe(Shader* vtxp, Shader* fragp);
    Pipe* CreatePipe(Shader* vtxp, Shader* fragp, Shader* geop);
}

#endif
