#ifndef CBPP_COLOR_H
#define CBPP_COLOR_H

#include <cstdint>

namespace cbpp {
	struct NormColor {
		float r, g, b, a;
	};
	
	struct Color {
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {};
		Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {};
		Color(uint8_t rgb, uint8_t a) : r(rgb), g(rgb), b(rgb), a(255) {};
		Color(uint8_t rgba) : r(rgba), g(rgba), b(rgba), a(rgba) {};
		
		Color(Color& clr) : r(clr.r), g(clr.g), b(clr.b), a(clr.a) {};
		
		Color() : r(0), g(0), b(0), a(0) {};
		
		Color operator+(Color other);
		Color operator+(uint8_t other);
		
		Color operator-(Color other);
		Color operator-(uint8_t other);
		
		Color operator*(float other);
		Color operator*(uint8_t other);
		
		Color operator/(float other);
		Color operator/(uint8_t other);
		
		NormColor Normalized() const noexcept;
		
		uint8_t r,g,b,a;
	};
}

#endif