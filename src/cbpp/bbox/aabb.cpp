#include "cbpp/bbox/aabb.h"
#include "cbpp/cb_alloc.h"

namespace cbpp{
	AABB::AABB(const AABB& other){
		Allocate();
		
		for(uint8_t i = 0; i < 4; i++){
			pts[i] = other.pts[i];
		}
	}
	
	AABB::AABB(){
		Set(Vec2(-5), Vec2(5));
	}
	
	AABB::AABB(Vec2 minp, Vec2 maxp){
		Allocate();
		
		Set(minp, maxp);
	}
	
	void AABB::Set(Vec2 minp, Vec2 maxp){
		if(!allocated){
			Allocate();
		}
		
		pts[0] = minp;
		pts[1] = Vec2(maxp.x, minp.y);
		pts[2] = maxp;
		pts[3] = Vec2(minp.x, maxp.y);
	}
	
	Vec2 AABB::At(uint8_t index){
		if(allocated){
			index = index % 4;
			return pts[index];
		}else{
			return Vec2(0);
		}
	}
	
	Vec2 AABB::operator[](uint8_t index){
		return At(index);
	}
	
	bool AABB::CollidePoint(Vec2 pt){
		if(allocated){
			return (pt.x >= At(0).x && pt.x <= At(1).x) && (pt.y >= At(0).y && pt.y <= At(2).y);
		}else{
			return false;
		}
	}
	
	bool AABB::CollideAABB(AABB& other){
		if(allocated){
			bool out = true;
			for(uint8_t i = 0; i < 4; i++){
				if(!other.CollidePoint(At(i))){
					out = false;
					break;
				}
			}
			
			return out;
		}else{
			return false;
		}
	}
	
	void AABB::Allocate(){
		if(allocated){
			Free();
		}
		
		pts = cbpp::Allocate<Vec2>(4);
		allocated = true;
	}
	
	void AABB::Free(){
		if(allocated){
			delete[] pts;
			pts = nullptr;
			
			allocated = false;
		}
	}
	
	AABB::~AABB(){
		Free();
	}
}