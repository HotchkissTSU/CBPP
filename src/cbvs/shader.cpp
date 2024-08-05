#include "cbvs/shader.h"

#include "cbpp/file_read.h"
#include "cbpp/error.h"

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <stdexcept>

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