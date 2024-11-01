#ifndef CBVS_ERROR_CHECK
#define CBVS_ERROR_CHECK

#include "cbpp/error.h"
#include "glad/glad.h"

namespace cbvs {
	GLenum CheckError(const char function[], const char file[], size_t line);
}

#define glCheck() cbvs::CheckError(__PRETTY_FUNCTION__, __FILE__, __LINE__)

#endif