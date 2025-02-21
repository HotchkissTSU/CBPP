#ifndef CBPP_VECTOR
#define CBPP_VECTOR

#include <stddef.h>

#include "cbpp/cbdef.h"

namespace cbpp{	
	struct Vec2 {
		float_t x,y;

		Vec2() : x(0.0f), y(0.0f) {};
		Vec2(const cbpp::Vec2& v) : x(v.x), y(v.y) {};

		template<typename NUM_T> Vec2(NUM_T _x, NUM_T _y) : x((float_t)_x), y((float_t)_y) {};
		template<typename NUM_T> Vec2(NUM_T _value) : x((float_t)_value), y((float_t)_value) {};
		
		float_t Distance(Vec2 other);
		
		float_t Length();
		Vec2 Norm();
		float_t Dot(Vec2 other);
		float_t Angle();
		
		Vec2 GetRotated(float_t ang);
		Vec2 GetRotated(int ang);
		void Rotate(float_t ang);
		void Rotate(int ang);
		
		float_t Project(Vec2 other);
		
		float_t Cross(Vec2 other);
		
		Vec2 operator+(Vec2 other) const noexcept;
		Vec2 operator-(Vec2 other) const noexcept;
		Vec2 operator*(Vec2 other) const noexcept;
		Vec2 operator/(Vec2 other) const noexcept;

		template <typename INT_T> Vec2 operator+(INT_T other) const noexcept {
			return Vec2( (float_t)(x + other), (float_t)(y + other) );
		}

		template <typename INT_T> Vec2 operator-(INT_T other) const noexcept {
			return Vec2( (float_t)(x - other), (float_t)(y - other) );
		}

		template <typename INT_T> Vec2 operator*(INT_T other) const noexcept {
			return Vec2( (float_t)(x * other), (float_t)(y * other) );
		}

		template <typename INT_T> Vec2 operator/(INT_T other) const noexcept {
			return Vec2( (float_t)(x / other), (float_t)(y / other) );
		}
		
		bool operator==(Vec2 other) noexcept;
		bool operator!=(Vec2 other) noexcept;
	};
}

#endif