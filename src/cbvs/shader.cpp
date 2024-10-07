#include "cbvs/shader.h"

#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include <stdio.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdexcept>
/*
using cbpp::ReadFileText;

namespace cbdraw {	
	void Shader::Load(const GLchar* vertex_path, const GLchar* frag_path, const GLchar* geom_path) {
		GLchar *vertex_src, *frag_src, *geom_src = NULL;
		FILE *vertex_in, *frag_in, *geom_in;
		
		//read vertex shader
		vertex_in = fopen(vertex_path, "r");
		if(vertex_in == NULL){
			//SetError("Shader Error", "Unable to open vertex shader source file");
			
			//log it
		}
		
		vertex_src = ReadFileText(vertex_in);
		fclose(vertex_in);
		
		//read fragment shader
		frag_in = fopen(frag_path, "r");
		if(frag_in == NULL){
			//SetError("Shader Error", "Unable to open fragment shader source file");
			fclose(vertex_in);
			//log it
		}
		
		frag_src = ReadFileText(frag_in);
		fclose(frag_in);
		
		if(geom_path != NULL){
			geom_in = fopen(geom_path, "r");
			if(geom_in != NULL){
				geom_src = ReadFileText(geom_in);
				fclose(geom_in);
			}else{
				//SetError("Shader Warning", "Unable to open geometry shader source file");
				//log it
			}
		}
		
		GLuint vertex, fragment, geometry;
		GLint success;
		GLchar info_log[512];
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_src, NULL);
		glCompileShader(vertex);
		
		delete[] vertex_src;
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(vertex, 512, NULL, info_log);
			//log it
		}
		
		memset(info_log, 0, 512);
		
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &frag_src, NULL);
		glCompileShader(fragment);
		
		delete[] frag_src;
		
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(fragment, 512, NULL, info_log);
			//log it
		}
		memset(info_log, 0, 512);
		
		bool has_geom = true;
		
		if(geom_src != NULL){
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geom_src, NULL);
			glCompileShader(geometry);
			
			delete[] geom_src;
			
			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if(!success){
				//log it
			}
		}
		
		ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, vertex);
		glAttachShader(ShaderProgram, fragment);
		if(has_geom){
			glAttachShader(ShaderProgram, geometry);
		}
		
		glLinkProgram(ShaderProgram);
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
		
		if(!success){
			glGetProgramInfoLog(ShaderProgram, 512, NULL, info_log);
			//log it
		}
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	
	GLuint Shader::GetProgramID(){ return ShaderProgram; }
	
	void Shader::Use(){ glUseProgram(ShaderProgram); }
}
*/

namespace cbvs { //utility
	std::map<std::string, ShaderProgram*> ShaderProgramRegistry;
	std::map<std::string, Shader*> ShaderRegistry;

	void RegisterShader(Shader* shader, std::string sh_name) {
		if(ShaderRegistry.count(sh_name) > 0) {
			UnregisterShader(sh_name);
		}

		ShaderRegistry[sh_name] = shader;
		shader->_setRegName(sh_name);
	}

	void UnregisterShader(std::string sh_name) {
		if(ShaderRegistry.count(sh_name) > 0) {
			ShaderRegistry.erase(sh_name);
		}
	}

	void RegisterProgram(ShaderProgram* shader, std::string sh_name) {
		if(ShaderProgramRegistry.count(sh_name) > 0) {
			UnregisterProgram(sh_name);
		}

		ShaderProgramRegistry[sh_name] = shader;
		shader->_setRegName(sh_name);
	}

	void UnregisterProgram(std::string sh_name) {
		if(ShaderProgramRegistry.count(sh_name) > 0) {
			ShaderProgramRegistry.erase(sh_name);
		}
	}

	Shader* GetShaderByName(std::string sh_name) {
		if(ShaderRegistry.count(sh_name) > 0) {
			return ShaderRegistry[sh_name];
		}else{
			return NULL;
		}
	}

	ShaderProgram* GetProgramByName(std::string prog_name) {
		if(ShaderProgramRegistry.count(prog_name) > 0) {
			return ShaderProgramRegistry[prog_name];
		}else{
			return NULL;
		}
	}

	void UseProgram(std::string prog_name) {
		if(ShaderProgramRegistry.count(prog_name) > 0) {
			ShaderProgramRegistry[prog_name]->Use();
		}
	}
}

namespace cbvs { //Shader
	Shader::Shader() {}
	
	Shader::Shader(GLenum sh_class, const char* src_path) {
		this->Load(sh_class, src_path);
	}

	bool Shader::Load(GLenum sh_class, const char* src_path) {
		cbpp::File input(src_path, "rt");

		if(!input.IsOpen()) {
			return false;
		}

		char* shader_src = new char[input.Length()+1];
		memset(shader_src, 0, input.Length()+1);
		input.Read(shader_src, input.Length());

		shader_obj_id = glCreateShader(sh_class);

		glShaderSource(shader_obj_id, 1, &shader_src, NULL);		

		glCompileShader(shader_obj_id);

		int compile_result;
		glGetShaderiv(shader_obj_id, GL_COMPILE_STATUS, &compile_result);

		if(!compile_result) {
			char log[512];
			glGetShaderInfoLog(shader_obj_id, 512, NULL, log);
			CbThrowError(log);
		}

		delete[] shader_src;
		return true;
	}

	const GLuint Shader::GetObjectID() {
		return shader_obj_id;
	}

	void Shader::_setRegName(std::string rname) {
		shader_name = rname;
	}

	Shader::~Shader() {
		UnregisterShader( shader_name );
		glDeleteShader( shader_obj_id );
	}
}

namespace cbvs { //ShaderProgram
	ShaderProgram::ShaderProgram(Shader* vtx, Shader* frag, Shader* geom = NULL) {
		this->SetShaders(vtx, frag, geom);
	}

	ShaderProgram::ShaderProgram(std::string vtx_name, std::string frag_name, std::string geom_name) {
		this->SetShaders(vtx_name, frag_name, geom_name);
	}

	bool ShaderProgram::SetShaders(Shader* vtx, Shader* frag, Shader* geom = NULL) {
		bool out = true;
		if(vtx == NULL || frag == NULL) {
			out = false;
			CbThrowError("Vertex or fragment shaders are missing!");
		}

		shader_prog_id = glCreateProgram();
		glAttachShader(shader_prog_id, vtx->GetObjectID());
		glAttachShader(shader_prog_id, frag->GetObjectID());
		if(geom != NULL) {
			glAttachShader(shader_prog_id, geom->GetObjectID());
		}

		return out;
	}

	bool ShaderProgram::SetShaders(std::string vtx_name, std::string frag_name, std::string geom_name) {
		return this->SetShaders(
			GetShaderByName(vtx_name),
			GetShaderByName(frag_name),
			GetShaderByName(geom_name)
		);
	}

	bool ShaderProgram::LinkProgram() {
		bool out;
		glLinkProgram(shader_prog_id);
		glGetProgramiv(shader_prog_id, GL_LINK_STATUS, (GLint*)( &out ));
		
		if(!out){
			char info_log[512];
			glGetProgramInfoLog(shader_prog_id, 512, NULL, info_log);
			CbThrowError(info_log);
		}

		shprog_linked = out;

		return out;
	}

	void ShaderProgram::Use() {
		glUseProgram(shader_prog_id);
	}

	void ShaderProgram::_setRegName(std::string sh_name) {
		shprog_name = sh_name;
	}

	GLint ShaderProgram::UniformLocation(const char* uname) {
		if(!shprog_linked) {
			CbThrowErrorf("Attempt to get uniform '%s' location from the non-linked shader program '%s'(%d)", uname, shprog_name.c_str(), shader_prog_id);
		}

		GLint uloc = glGetUniformLocation(shader_prog_id, uname);
		if(uloc == -1) {
			CbThrowErrorf("Failed to get uniform '%s' location from the shader program '%s'(%u)", uname, shprog_name.c_str(), shader_prog_id);
		}

		return uloc;
	}

	void ShaderProgram::PushUniform(const char* uname, float v1, float v2, float v3, float v4) {
		if(!shprog_linked) {
			CbThrowError("Attempt to push uniform value to the non-linked shader program");
		}

		GLint uloc = this->UniformLocation(uname);

		glUniform4f(uloc, v1, v2, v3, v4);
	}

	ShaderProgram::~ShaderProgram() {
		UnregisterProgram( shprog_name );
	}
}