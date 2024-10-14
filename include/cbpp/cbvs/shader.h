#ifndef CBVS_SHADER_H
#define CBVS_SHADER_H

#include "glad/glad.h"

#include <cstdint>
#include "cbpp/cbstring.h"

#include <map>
#include <stdio.h>

namespace cbvs {
	extern const char *default_vtx, *default_frag;

	class Shader {
		public:
			Shader() = default;
			Shader(const char* source);

	};
}

#endif
