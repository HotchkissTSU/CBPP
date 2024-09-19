#ifndef CBPP_VECTOR
#define CBPP_VECTOR

#include <math.h>
#include <iostream>
#include <cstdint>

namespace cbpp{	
	extern "C" struct Vec2{
		float x,y;
	
		Vec2(float xv, float yv);
		Vec2(float v);
		Vec2();
		Vec2(const cbpp::Vec2& v);
		Vec2(int xv, int yv);
		Vec2(int v);
		
		float GetX();
		float GetY();
		
		void SetX(float xv);
		void SetY(float yv);
		void Set(float xv, float yv);
		
		float Distance(Vec2 other);
		
		float Length();
		Vec2 Norm();
		float Dot(Vec2 other);
		Vec2 Cross(Vec2 other);
		float Angle();
		
		Vec2 GetRotated(float ang);
		Vec2 GetRotated(int ang);
		void Rotate(float ang);
		void Rotate(int ang);
		
		float Project(Vec2 other);
		
		float VectorMul(Vec2 other);
		
		Vec2 operator+(Vec2 other);
		Vec2 operator+(float other);
		Vec2 operator+(int other);
		
		Vec2 operator-(Vec2 other);
		Vec2 operator-(float other);
		Vec2 operator-(int other);
		
		Vec2 operator*(Vec2 other);
		Vec2 operator*(float other);
		Vec2 operator*(int other);
		
		Vec2 operator/(Vec2 other);
		Vec2 operator/(float other);
		Vec2 operator/(int other);
		
		bool operator==(Vec2 other);
		bool operator!=(Vec2 other);
		float operator[](std::size_t index);
		friend std::ostream& operator << (std::ostream &o, Vec2 const& t);
	};
	
	extern "C" float Clamp(float v, float min, float max);
	extern "C" Vec2 ClampVector(Vec2 v, float min, float max);
}

#endif