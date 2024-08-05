#ifndef CBPP_IMAGE_H
#define CBPP_IMAGE_H

#include "png/png.h"
#include <cstdint>

namespace cbpp {
	struct Image {
		uint64_t length = 0;
		uint8_t** bytes = nullptr;
	};
	
	Image LoadImage(const char* path);
	void FreeImage(Image& target);
}

#endif