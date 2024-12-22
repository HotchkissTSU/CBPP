#include "cbpp/vec2.h"

namespace cbpp{
	Vec2::Vec2(const cbpp::Vec2& v){ x = v.x; y = v.y; }
	Vec2::Vec2(float_t xv, float_t yv){ x = xv; y = yv; }
	Vec2::Vec2(float_t v){ x = v; y = v; }
	Vec2::Vec2(){ x = 0.0f; y = 0.0f; }
	Vec2::Vec2(int xv, int yv){ x = (float_t)xv; y = (float_t)yv; }
	Vec2::Vec2(int v){ x = (float_t)v; y = (float_t)v; }
	
	float_t Vec2::GetX(){ return x; }
	float_t Vec2::GetY(){ return y; }
	
	void Vec2::SetX(float_t xv){ x = xv; }
	void Vec2::SetY(float_t yv){ y = yv; }
	void Vec2::Set(float_t xv, float_t yv){ x = xv; y = yv; }
	
	float_t Vec2::Distance(Vec2 other){ return (*this - other).Length(); }
	
	float_t Vec2::Length(){ return std::pow(std::pow(x, 2.0f) + std::pow(y, 2.0f), 0.5f); }
	Vec2 Vec2::Norm(){ return (*this/this->Length()); }
	float_t Vec2::Dot(Vec2 other){ Vec2 v1 = other.Norm(); Vec2 v2 = this->Norm(); return (v1.x*v2.x + v1.y*v2.y); }
	
	float_t Vec2::Angle(){
		int addon;
		if(x >= 0 && y >= 0){ //1
			addon = 0;
		}else if(x >= 0 && y < 0){//4       переделай это в радианы
			addon = -90;
		}else if(x < 0 && y >= 0){//2
			addon = 90;
		}else if(x < 0 && y < 0){//3
			addon = 180;
		}
		
		return acos(this->Dot( Vec2(1.0f,0.0f) )) + addon;
	}
	
	Vec2 Vec2::GetRotated(float_t ang){
		float_t nx = x*std::cos(ang) - y*std::sin(ang);
		float_t ny = x*std::sin(ang) + y*std::cos(ang);
		return Vec2(nx, ny);
	}
	
	Vec2 Vec2::GetRotated(int ang){
		return GetRotated((float_t)ang);
	}
	
	void Vec2::Rotate(float_t ang){
		x = x*std::cos(ang) - y*std::sin(ang);
		y = x*std::sin(ang) + y*std::cos(ang);
	}
	
	void Vec2::Rotate(int ang){
		Rotate((float_t)ang);
	}
	
	float_t Vec2::Project(Vec2 other){
		return (x*y + other.x*other.y)/other.Length();
	}
	
	float_t Vec2::Cross(Vec2 other){
		return x*other.y - y*other.x;
	}
	
	Vec2 Vec2::operator+(Vec2 other) const noexcept { return Vec2(x + other.x, y + other.y); }
	Vec2 Vec2::operator-(Vec2 other) const noexcept { return Vec2(x - other.x, y - other.y); }
	Vec2 Vec2::operator*(Vec2 other) const noexcept { return Vec2(x * other.x, y * other.y); }
	Vec2 Vec2::operator/(Vec2 other) const noexcept {
		float_t ox,oy;
		if(other.x == 0){
			ox = 0;
		}else{
			ox = x / other.x;
		}
		if(other.y == 0){
			oy = 0;
		}else{
			oy = y / other.y;
		}
		return Vec2(ox,oy); 
	}
	
	bool Vec2::operator==(Vec2 other) noexcept { return ((x == other.x) && (y == other.y)); }
	bool Vec2::operator!=(Vec2 other) noexcept { return ((x != other.x) || (y != other.y)); }
	
	float_t Vec2::operator[](std::size_t index){ if(index == 0){ return x; }else{ return y; } }
}

namespace cbpp{	
	Vec2 ClampVector(Vec2 v, float_t min, float_t max){
		return v.Norm()*Clamp(v.Length(), min, max);
	}
}