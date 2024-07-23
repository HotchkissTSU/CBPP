#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include <cstdint>
#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace cbpp {
	class Shader {
		public:
			Shader(const GLchar* vertex_path, const GLchar* frag_path);
			void Use();
			
			GLuint GetProgramID();
		private:
			GLuint ShaderProgram = 0;
	};
}

#endif