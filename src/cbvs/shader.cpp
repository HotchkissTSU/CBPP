#include "cbvs/shader.h"

namespace cbvs {
const char* default_vtx = "#version 400 core\
layout(location = 0) in vec2 inpos;\
void main() {\
    gl_Position = vec4(inpos, 0.0, 0.0);\
}";

const char* default_frag = "#version 400 core\
uniform vec4 ddraw_COLOR;\
void main() {\
    gl_FragColor = ddraw_COLOR;\
}";

	
}