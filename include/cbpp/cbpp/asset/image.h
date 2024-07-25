#ifndef CBPP_IMAGE_H
#define CBPP_IMAGE_H

#include "png/png.h"
#include <cstdint>

namespace cbpp {
	class Image {
		public:
			Image(const char* filepath);
			
			uint32_t Width();
			uint32_t Height();
			
			bool Valid() { return valid; };
			
		private:
			bool valid = false;
			png_bytepp img_rows = NULL;
			
			png_infop img_info = NULL;
			png_structp img_base = NULL;
	};
}

#endif