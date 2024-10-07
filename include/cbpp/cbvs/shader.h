#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include "glad/glad.h"

#include <cstdint>
#include "cbpp/cbstring.h"

#include <map>
#include <stdio.h>

namespace cbvs {
	class Shader {
		public:
			Shader();
			Shader(GLenum sh_class, const char* src_path);
			
			bool Load(GLenum sh_class, const char* src_path);
			GLenum Type();

			const GLuint GetObjectID();
			
			void _setRegName(std::string name);

			~Shader();
			
		private:
			GLenum shader_class = GL_VERTEX_SHADER;
			std::string shader_name;
			GLuint shader_obj_id;
	};
	
	class ShaderProgram {
		public:
			ShaderProgram() {};
			ShaderProgram(Shader* vtx, Shader* frag, Shader* geom);
			ShaderProgram(std::string vtx_name, std::string frag_name, std::string geom_name);
			
			bool SetShaders(Shader* vtx, Shader* frag, Shader* geom);
			bool SetShaders(std::string vtx_name, std::string frag_name, std::string geom_name);
			bool LinkProgram();

			void Use();

			GLint UniformLocation(const char* uname);
			void PushUniform(const char* uname, float v1, float v2, float v3, float v4);

			void _setRegName(std::string sh_name);

			~ShaderProgram();
			
		private:
			GLuint shader_prog_id;
			std::string shprog_name;
			bool shprog_linked = false;
	};

	extern std::map<std::string, ShaderProgram*> ShaderProgramRegistry;
	extern std::map<std::string, Shader*> ShaderRegistry;

	Shader* GetShaderByName(std::string sh_name);
	void UseProgram(std::string prog_name);

	void RegisterShader(Shader* shader, std::string sh_name);
	void UnregisterShader(std::string sh_name);

	void RegisterProgram(ShaderProgram* shader, std::string sh_name);
	void UnregisterProgram(std::string sh_name);
}

#endif
