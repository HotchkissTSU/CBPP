#include "cbpp/color.h"

#include "cbpp/geomath.h"

using namespace cbpp::math;

namespace cbpp {	
	Color Color::operator+(Color other) {
		return Color(
			Clamp(r+other.r, 0, 255),
			Clamp(g+other.g, 0, 255),
			Clamp(b+other.b, 0, 255),
			Clamp(a+other.a, 0, 255)
		);
	}
	
	Color Color::operator+(uint8_t other) {
		return Color(
			Clamp(r+other, 0, 255),
			Clamp(g+other, 0, 255),
			Clamp(b+other, 0, 255),
			Clamp(a+other, 0, 255)
		);
	}
	
	Color Color::operator-(Color other) {
		return Color(
			Clamp(r-other.r, 0, 255),
			Clamp(g-other.g, 0, 255),
			Clamp(b-other.b, 0, 255),
			Clamp(a-other.a, 0, 255)
		);
	}
	
	Color Color::operator-(uint8_t other) {
		return Color(
			Clamp(r-other, 0, 255),
			Clamp(g-other, 0, 255),
			Clamp(b-other, 0, 255),
			Clamp(a-other, 0, 255)
		);
	}
	
	Color Color::operator*(float other) {
		return Color(
			(uint8_t)Clamp(r*other, 0.0f, 255.0f),
			(uint8_t)Clamp(g*other, 0.0f, 255.0f),
			(uint8_t)Clamp(b*other, 0.0f, 255.0f),
			(uint8_t)Clamp(a*other, 0.0f, 255.0f)
		);
	}
	
	Color Color::operator*(uint8_t other) {
		return operator*((float)other);
	}
	
	Color Color::operator/(float other) {
		if(math::Equal(other, 0.0f)) { return Color(255); }
		
		return Color(
			(uint8_t)Clamp(r/other, 0.0f, 255.0f),
			(uint8_t)Clamp(g/other, 0.0f, 255.0f),
			(uint8_t)Clamp(b/other, 0.0f, 255.0f),
			(uint8_t)Clamp(a/other, 0.0f, 255.0f)
		);
	}
	
	Color Color::operator/(uint8_t other) {
		return operator/((float)other);
	}
	
	NormColor Color::Normalized() {
		NormColor out;
		
		out.r = (float)r / 255.0f;
		out.g = (float)g / 255.0f;
		out.b = (float)b / 255.0f;
		out.a = (float)a / 255.0f;
		
		return out;
	}
}
