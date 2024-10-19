#include "cbvs/shader.h"
#include "cbpp/error.h"
#include "cbpp/math.h"

#if !defined(CBVS_SHADERS_STRICT)
    #define CBVS_SHADERS_STRICT 1
#endif

#if CBVS_SHADERS_STRICT == CBVS_STRICT
    #define CBVS_SHADER_ERR(reason) {CbThrowError(reason);}
    #define CBVS_SHADER_ERRF(reason, ...) {CbThrowErrorf(reason, __VA_ARGS__);}
#endif

#if CBVS_SHADERS_STRICT == CBVS_TOLERANT
    #define CBVS_SHADER_ERR(reason) {CbThrowWarning(reason);}
    #define CBVS_SHADER_ERRF(reason, ...) {CbThrowWarningf(reason, __VA_ARGS__);}
#endif

namespace cbvs {
const char* default_vtx = "#version 400 core\n\
layout(location = 0) in vec2 inpos;\
void main() {\
    gl_Position = vec4(inpos, 0.0, 0.0);\
}";

const char* default_frag = "#version 400 core\n\
uniform vec4 ddraw_COLOR;\
void main() {\
    gl_FragColor = ddraw_COLOR;\
}";
}

namespace cbvs {
    Shader::Shader( GLenum type, const char* src ) {
        Compile(type, src);
    }

    bool Shader::Compile( GLenum type, const char* src ) {
        objid = glCreateShader(type);
        if(!glIsShader(objid)) {
            CBVS_SHADER_ERR("Failed to create OpenGL shader object");
            return false;
        }

        glShaderSource( objid, 1, &src, NULL );
        glCompileShader( objid );

        GLint result = GL_FALSE;
        glGetShaderiv( objid, GL_COMPILE_STATUS, &result );

        if(result != GL_TRUE) {
            GLint logSize = 0;
            glGetShaderiv(objid, GL_INFO_LOG_LENGTH, &logSize);
            
            char* inflog = new char[logSize]();

            glGetShaderInfoLog(objid, 255, NULL, inflog);
            CBVS_SHADER_ERRF("%s", inflog);
            delete[] inflog;
            return false;
        }

        compiled = true;
        return true;
    }
    
    GLuint Shader::ID() { return objid; }

    bool Shader::IsValid() {
        if(!compiled) { return false; }

        return true;
    }

    Shader::~Shader() {
        glDeleteShader(objid);
    }
}

namespace cbvs {
    void Pipe::Create() {
        objid = glCreateProgram();
        if(!glIsProgram(objid)) {
            CBVS_SHADER_ERR("Failed to create OpenGL shader program object");
        }
    }

    Pipe::Pipe(Shader** sharr, size_t ln) {
        this->Create();
        this->Link(sharr, ln);
    }

    Pipe::Pipe(Shader* vtxp, Shader* fragp) {
        this->Create();

        Shader* buf[2] = {
            vtxp, fragp
        };

        this->Link(buf, 2);
    }

    Pipe::Pipe(Shader* vtxp, Shader* fragp, Shader* geop) {
        this->Create();

        Shader* buf[3] = {
            vtxp, fragp, geop
        };

        this->Link(buf, 3);
    }

    bool Pipe::Link(Shader** sharr, size_t ln) {
        if(linked) {
            CBVS_SHADER_ERRF("Pipe %x is already linked", this);
            return false;
        }

        if( ln > 5 ) {
            CBVS_SHADER_ERRF("Too many shaders are passed to the pipe %x: %lu with maximum of 5", this, ln);
            return false;
        }

        size_t len = cbpp::Clamp(ln, 0, 5);
        for(size_t i = 0; i < len; i++) {
            Shader* ptr = sharr[i];
            glAttachShader(objid, ptr->ID());
        }

        glLinkProgram(objid);
        GLint result = GL_FALSE;
        glGetProgramiv(objid, GL_LINK_STATUS, &result);
        
        if(result != GL_TRUE) {
            GLint logSize = 0;
            glGetProgramiv(objid, GL_INFO_LOG_LENGTH, &logSize);

            char* log = new char[logSize]();
            glGetProgramInfoLog(objid, logSize, NULL, log);

            CBVS_SHADER_ERRF("%s", log);

            return false;
        }

        linked = true;
        return true;
    }

    void Pipe::Use() { glUseProgram(objid); }

    GLuint Pipe::ID() { return objid; }

    bool Pipe::IsValid() {
        if( !linked ) { return false; }

        return true;
    }

    Pipe::~Pipe() {
        glDeleteProgram(objid);
    }
}