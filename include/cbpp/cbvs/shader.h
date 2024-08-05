#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include <cstdint>
#include "cbpp_gl.h"
#include "GLFW/glfw3.h"

#define CBVS_SHADER_LOG_PATH "logs/shaders"

namespace cbdraw {
	class Shader {
		public:
			void Load(const GLchar* vertex_path, const GLchar* frag_path, const GLchar* geom_path = NULL);
			void Use();
			
			GLuint GetProgramID();
		private:
			GLuint ShaderProgram = 0;
	};
	
	
}

#endif