#ifndef CBPP_IMAGE_H
#define CBPP_IMAGE_H

#include "cbpp/cbstring.h"

#include <cstdint>

namespace cbpp {
	struct Image {
		uint64_t length = 0;
		uint8_t** bytes = nullptr;
		
		uint32_t width = 0, height = 0;
	};
	
	bool LoadImage(String path, Image& target);
	void FreeImage(Image& target);
}

#endif