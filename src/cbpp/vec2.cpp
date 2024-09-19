//#include "pch.h"
#include "cbpp/vec2.h"

namespace cbpp{
	Vec2::Vec2(const cbpp::Vec2& v){ x = v.x; y = v.y; }
	Vec2::Vec2(float xv, float yv){ x = xv; y = yv; }
	Vec2::Vec2(float v){ x = v; y = v; }
	Vec2::Vec2(){ x = 0.0f; y = 0.0f; }
	Vec2::Vec2(int xv, int yv){ x = (float)xv; y = (float)yv; }
	Vec2::Vec2(int v){ x = (float)v; y = (float)v; }
	
	float Vec2::GetX(){ return x; }
	float Vec2::GetY(){ return y; }
	
	void Vec2::SetX(float xv){ x = xv; }
	void Vec2::SetY(float yv){ y = yv; }
	void Vec2::Set(float xv, float yv){ x = xv; y = yv; }
	
	float Vec2::Distance(Vec2 other){ return (*this - other).Length(); }
	
	float Vec2::Length(){ return std::pow(std::pow(x, 2.0f) + std::pow(y, 2.0f), 0.5f); }
	Vec2 Vec2::Norm(){ return (*this/this->Length()); }
	float Vec2::Dot(Vec2 other){ Vec2 v1 = other.Norm(); Vec2 v2 = this->Norm(); return (v1.x*v2.x + v1.y*v2.y); }
	
	float Vec2::Angle(){
		int addon;
		//арккосинус - чмо, просирает, в какой четверти был угол, чистим за ним
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
	
	Vec2 Vec2::GetRotated(float ang){
		float nx = x*std::cos(ang) - y*std::sin(ang);
		float ny = x*std::sin(ang) + y*std::cos(ang);
		return Vec2(nx, ny);
	}
	
	Vec2 Vec2::GetRotated(int ang){
		return GetRotated((float)ang);
	}
	
	void Vec2::Rotate(float ang){
		x = x*std::cos(ang) - y*std::sin(ang);
		y = x*std::sin(ang) + y*std::cos(ang);
	}
	
	void Vec2::Rotate(int ang){
		Rotate((float)ang);
	}
	
	float Vec2::Project(Vec2 other){
		return (x*y + other.x*other.y)/other.Length();
	}
	
	float Vec2::VectorMul(Vec2 other){
		return x*other.y - y*other.x;
	}
	
	Vec2 Vec2::operator+(Vec2 other){ return Vec2(x + other.x, y + other.y); }
	Vec2 Vec2::operator+(float other){ return Vec2(x + other, y + other); }
	Vec2 Vec2::operator+(int other){ return Vec2(x + (float)other, y + (float)other); }
	
	Vec2 Vec2::operator-(Vec2 other){ return Vec2(x - other.x, y - other.y); }
	Vec2 Vec2::operator-(float other){ return Vec2(x - other, y - other); }
	Vec2 Vec2::operator-(int other){ return Vec2(x - (float)other, y - (float)other); }
	
	Vec2 Vec2::operator*(Vec2 other){ return Vec2(x * other.x, y * other.y); }
	Vec2 Vec2::operator*(float other){ return Vec2(x * other, y * other); }
	Vec2 Vec2::operator*(int other){ return Vec2(x * (float)other, y * (float)other); }
	
	Vec2 Vec2::operator/(Vec2 other){
		float ox,oy;
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
	
	Vec2 Vec2::operator/(float other){
		if(other == 0.0f){
			return Vec2();
		}else{
			return Vec2(x/other,y/other);
		}
	}
	
	Vec2 Vec2::operator/(int other){ return *this/(float)other; }
	
	bool Vec2::operator==(Vec2 other){ return ((x == other.x) && (y == other.y)); }
	bool Vec2::operator!=(Vec2 other){ return ((x != other.x) || (y != other.y)); }
	
	float Vec2::operator[](std::size_t index){ if(index == 0){ return x; }else{ return y; } }
	
	std::ostream& operator << (std::ostream &o, Vec2 const& t){ return o<<"Vec2("<<(float)t.x<<", "<<(float)t.y<<")"; }
}

namespace cbpp{	
	extern "C" Vec2 ClampVector(Vec2 v, float min, float max){
		return v.Norm()*Clamp(v.Length(), min, max);
	}
}