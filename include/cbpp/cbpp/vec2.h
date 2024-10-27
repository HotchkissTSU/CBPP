#ifndef CBPP_VECTOR
#define CBPP_VECTOR

#include "cbpp/cbdef.h"
#include <math.h>

namespace cbpp{	
	struct Vec2{
		float_t x,y;
	
		Vec2(float_t xv, float_t yv);
		Vec2(float_t v);
		Vec2();
		Vec2(const cbpp::Vec2& v);
		Vec2(int xv, int yv);
		Vec2(int v);
		
		float_t GetX();
		float_t GetY();
		
		void SetX(float_t xv);
		void SetY(float_t yv);
		void Set(float_t xv, float_t yv);
		
		float_t Distance(Vec2 other);
		
		float_t Length();
		Vec2 Norm();
		float_t Dot(Vec2 other);
		Vec2 Cross(Vec2 other);
		float_t Angle();
		
		Vec2 GetRotated(float_t ang);
		Vec2 GetRotated(int ang);
		void Rotate(float_t ang);
		void Rotate(int ang);
		
		float_t Project(Vec2 other);
		
		float_t VectorMul(Vec2 other);
		
		Vec2 operator+(Vec2 other) noexcept;
		Vec2 operator+(float_t other) noexcept;
		Vec2 operator+(int other) noexcept;
		
		Vec2 operator-(Vec2 other) noexcept;
		Vec2 operator-(float_t other) noexcept;
		Vec2 operator-(int other) noexcept;
		
		Vec2 operator*(Vec2 other) noexcept;
		Vec2 operator*(float_t other) noexcept;
		Vec2 operator*(int other) noexcept;
		
		Vec2 operator/(Vec2 other) noexcept;
		Vec2 operator/(float_t other) noexcept;
		Vec2 operator/(int other) noexcept;
		
		bool operator==(Vec2 other) noexcept;
		bool operator!=(Vec2 other) noexcept;

		float_t operator[](std::size_t index);
	};
	
	float_t Clamp(float_t v, float_t min, float_t max);
	Vec2 ClampVector(Vec2 v, float_t min, float_t max);
}

#endif