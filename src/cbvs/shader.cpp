#include "cbvs/shader.h"
#include "cbpp/file_read.h"

#include <stdio.h>
#include <cstring>
#include <iostream>

namespace cbpp {
	Shader::Shader(const GLchar* vertex_path, const GLchar* frag_path) {
		GLchar *vertex_src, *frag_src;
		FILE *vertex_in, *frag_in;
		
		//read vertex shader
		vertex_in = fopen(vertex_path, "r");
		if(vertex_in == NULL){
			//log it
		}
		
		vertex_src = ReadFileText(vertex_in);
		
		//read fragment shader
		frag_in = fopen(frag_path, "r");
		if(frag_in == NULL){
			//log it
		}
		
		frag_src = ReadFileText(frag_in);
		
		GLuint vertex, fragment;
		GLint success;
		GLchar info_log[512];
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_src, NULL);
		glCompileShader(vertex);
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(vertex, 512, NULL, info_log);
			//log it
		}
		
		memset(info_log, 0, 512);
		
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &frag_src, NULL);
		glCompileShader(fragment);
		
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(fragment, 512, NULL, info_log);
			//log it
		}
		memset(info_log, 0, 512);
		
		ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, vertex);
		glAttachShader(ShaderProgram, fragment);
		
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