#include "cbpp/asset/image.h"

#include "cbpp/error.h"

#include <stdio.h>
#include <stdexcept>

namespace cbpp {
	Image LoadImage(const char* path) {
		FILE* inp = fopen(path, "rb");
		if(inp == NULL) {
			//log it
		}
		
		uint8_t header[8];
		fread(header, 1, 8, inp);
		bool is_png = png_sig_cmp(header, 0, 8);
	}
}