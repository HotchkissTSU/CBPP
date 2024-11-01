#include "cbvs/error_check.h"

#include "glad/glad.h"

namespace cbvs {
    GLenum CheckError(const char function[], const char file[], size_t line) {
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR) {
		const char *error = "";
		switch (errorCode) {
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM";     break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE";     break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION";    break;
			//case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW";    break;
			//case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW";    break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY";     break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		CbThrowErrorf("OpenGL: %s, Line: %i, Function: %s\n", error, line, function);
		}

		return errorCode;
	}
}