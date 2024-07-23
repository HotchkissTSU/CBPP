#ifndef CBPP_COLOR_H
#define CBPP_COLOR_H

#include <cstdint>

namespace cbpp {
	struct Color {
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(uint8_t rgb, uint8_t a);
		Color(uint8_t rgba);
		
		uint8_t r,g,b,a;
	};
}

#endif