#include "cbvs/shader.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#if !defined(CBVS_SHADERS_STRICT)
    #define CBVS_SHADERS_STRICT CBVS_STRICT
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
uniform float cbpp_RATIO;\
void main() {\
    gl_Position = vec4(inpos.x, inpos.y, 0.0, 1.0);\
}";

const char* default_frag = "#version 400 core\n\
uniform vec4 cbpp_COLOR;\
void main() {\
    gl_FragColor = cbpp_COLOR;\
}";
}

namespace cbvs {
    Shader::Shader( GLenum type, const char* src ) {
        char prefix[16], sh_name[64];
        switch(type) {
            case GL_VERTEX_SHADER:
                snprintf(prefix, 16, "VTX");
            case GL_FRAGMENT_SHADER:
                snprintf(prefix, 16, "FRAG");
            case GL_GEOMETRY_SHADER:
                snprintf(prefix, 16, "GEOM");
            default:
                snprintf(prefix, 16, "UNDEF");
        }

        snprintf(sh_name, 64, "%s_unnamed", prefix);

        SetName(sh_name);
        Compile(type, src);
    }

    Shader::Shader( const char* name, GLenum type, const char* src ) {
        SetName(name);
        Compile(type, src);
    }

    void Shader::SetName(const char* shname) {
        snprintf(name, 64, "%s", shname);
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

    const char* Shader::Name() {
        return const_cast<const char*>(name);
    }

    bool Shader::IsValid() {
        if(!compiled) { return false; }

        GLint res = 0;
        glGetShaderiv(objid, GL_COMPILE_STATUS, &res);

        return (bool)res;
    }

    Shader::~Shader() {
        if(glIsShader(objid)){
            glDeleteShader(objid);
        }
    }
}

namespace cbvs {
    void Pipe::Create() {
        objid = glCreateProgram();
        if(!glIsProgram(objid)) {
            CBVS_SHADER_ERR("Failed to create OpenGL shader program object");
        }
    }

    Pipe::Pipe(PipeData& data) {
        this->Link(data);
    }

    Pipe::Pipe(Shader* vtxp, Shader* fragp) {
        PipeData buf = {
            vtxp, fragp, NULL
        };

        this->Link(buf);
    }
    
    Pipe::Pipe(Shader* vtxp, Shader* fragp, Shader* geop) {
        PipeData buf = {
            vtxp, fragp, geop
        };

        this->Link(buf);
    }

    bool Pipe::Link(PipeData& data) {
        Create();

        if(linked) {
            CBVS_SHADER_ERRF("Pipe %x is already linked", this);
            return false;
        }

        if(data.vtx) { 
            if(data.vtx->IsValid()) { glAttachShader(objid, data.vtx->ID()); }
            else{ CBVS_SHADER_ERRF("Shader '%s', passed as VTX shader, is invalid", data.vtx->Name()); }
        }

        if(data.frag) { 
            if(data.frag->IsValid()) { glAttachShader(objid, data.frag->ID()); }
            else{ CBVS_SHADER_ERRF("Shader '%s', passed as FRAG shader, is invalid", data.frag->Name()); }
        }

        if(data.geom) { 
            if(data.geom->IsValid()) { glAttachShader(objid, data.geom->ID()); }
            else{ CBVS_SHADER_ERRF("Shader '%s', passed as GEOM shader, is invalid", data.geom->Name()); }
        }

        glLinkProgram(objid);
        GLint result = GL_FALSE;
        glGetProgramiv(objid, GL_LINK_STATUS, &result);
        
        if(result != GL_TRUE) {
            GLint logSize = 0;
            glGetProgramiv(objid, GL_INFO_LOG_LENGTH, &logSize);

            char* log = new char[logSize]();
            glGetProgramInfoLog(objid, logSize, NULL, log);

            CBVS_SHADER_ERRF("Pipe %x error: %s", this, log);

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

    GLint Pipe::GetUniform(const char* uname) {
        GLint out = glGetUniformLocation(objid, uname);
        if(out == -1) {
            CBVS_SHADER_ERRF("Unable to get uniform '%s' from the pipe", uname);
        }

        return out;
    }

    void Pipe::PushUniform(const char* name, float_t a, float_t b, float_t c, float_t d) {
        GLint loc = GetUniform(name);
        glUniform4f(loc, (GLfloat)a, (GLfloat)b, (GLfloat)c, (GLfloat)d);
    }

    void Pipe::PushUniform(const char* name, float_t a) {
        GLint loc = GetUniform(name);
        glUniform1f(loc, (GLfloat)a);
    }

    void Pipe::PushUniform(const char* name, int32_t a) {
        GLint loc = GetUniform(name);
        glUniform1i(loc, (GLint)a);
    }

    Pipe::~Pipe() {
        glDeleteProgram(objid);
    }
}

namespace cbvs {
    Shader* DefaultVTX = nullptr;
    Shader* DefaultFRAG = nullptr;

    Pipe* DefaultPipe = nullptr;

    bool InitDefaultShaders() {
        DefaultVTX = new Shader( GL_VERTEX_SHADER, default_vtx );
        DefaultFRAG = new Shader( GL_FRAGMENT_SHADER, default_frag );

        PipeData sharr = {
            DefaultVTX, DefaultFRAG, NULL
        };

        DefaultPipe = new Pipe( sharr );

        printf("\nDefault VTX: %x;\nDefault FRAG: %x;\nDefault Pipeline: %x;\n", DefaultVTX, DefaultFRAG, DefaultPipe);

        return DefaultVTX->IsValid() && DefaultFRAG->IsValid() && DefaultPipe->IsValid();
    }

    void CleanupDefaultShaders() {
        if( DefaultVTX != nullptr )  { delete DefaultVTX;  }
        if( DefaultFRAG != nullptr ) { delete DefaultFRAG; }
        if( DefaultPipe != nullptr ) { delete DefaultPipe; }
    }

    const char* LoadShader(const char* path, GLenum type) {
        char filen[512] = {"assets/shaders/"};
        strcat(filen, path);

        switch(type) {
            case GL_VERTEX_SHADER:
                strcat(filen, ".vertex");
            case GL_FRAGMENT_SHADER:
                strcat(filen, ".fragment");
            case GL_GEOMETRY_SHADER:
                strcat(filen, ".geometry");
        }

        cbpp::File inp(filen, "rt");

        if(!inp.IsOpen()) {
            switch(type) {
                case GL_VERTEX_SHADER:
                    return default_vtx;
                case GL_FRAGMENT_SHADER:
                    return default_frag;
                default:
                    CBVS_SHADER_ERR("Unable to open source file nor fallback to the default shader source");
                    return NULL;
            }
        }

        char* buffer = new char[ inp.Length() + 1 ]();
        inp.Read(buffer, inp.Length());
        //inp.Close();

        return const_cast<const char*>( buffer );
    }

    Shader* CreateShader(GLenum type, const char* src) {
        Shader* out = new Shader();
        if(out->Compile(type, src)) {
            return out;
        }else{
            delete out;
            #if CBVS_SHADERS_STRICT == CBVS_STRICT
                CBVS_SHADER_ERR("Unable to create a new shader");
            #endif

            #if CBVS_SHADERS_STRICT == CBVS_TOLERANT
                CBVS_SHADER_ERR("Unable to create a new shader, trying to fallback...");
            #endif

            switch(type) {
                case GL_VERTEX_SHADER:
                    return DefaultVTX;
                
                case GL_FRAGMENT_SHADER:
                    return DefaultFRAG;

                case GL_GEOMETRY_SHADER:
                    CBVS_SHADER_ERR("There is no default fallback for geometry shaders");
                    return nullptr;

                default:
                    delete out;
                    CBVS_SHADER_ERR("Unable to create a new shader nor fallback to the default one");
                    return nullptr;
            }

            return out;
        }
    }

    Pipe* CreatePipe(PipeData& data) {
        Pipe* out = new Pipe();

        if(out->Link(data)) {
            return out;
        }else {
            if(DefaultPipe == nullptr) {
                CBVS_SHADER_ERR("Unable to create a new pipe nor fallback to the default one");
                return nullptr;
            }

            return DefaultPipe;
        }
    }

    Pipe* CreatePipe(Shader* vtxp, Shader* fragp) {
        PipeData sharr = {
            vtxp, fragp, NULL
        };
        return CreatePipe(sharr);
    }

    Pipe* CreatePipe(Shader* vtxp, Shader* fragp, Shader* geop) {
        PipeData sharr = {
            vtxp, fragp, geop
        };
        return CreatePipe(sharr);
    }
}