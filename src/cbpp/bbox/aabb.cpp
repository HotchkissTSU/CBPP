#include "cbpp/bbox/aabb.h"

using namespace cbpp;

namespace cbpp{
	AABB::AABB(){}
	AABB::AABB(Vec2 minp, Vec2 maxp){
		pmin = minp; pmax = maxp;
		GenerateBoxVector();
	}
	
	Vec2 AABB::GetMin(){ return pmin; }
	Vec2 AABB::GetMax(){ return pmax; }
	
	void AABB::Set(Vec2 p1, Vec2 p2){
		pmin = p1;
		pmax = p2;
		GenerateBoxVector();
	}
	
	void AABB::SetMin(Vec2 p){
		pmin = p;
		GenerateBoxVector();
	}
	
	void AABB::SetMax(Vec2 p){
		pmax = p;
		GenerateBoxVector();
	}
	
	std::vector<Vec2> AABB::GetBoxVector(){
		return boxv;
	}
	
	void AABB::GenerateBoxVector(){
		boxv[0] = pmin;
		boxv[1] = Vec2(pmax.x, pmin.y);
		boxv[2] = pmax;
		boxv[3] = Vec2(pmin.x, pmax.y);
	}
	
	bool AABB::CollidePoint(Vec2 p){
		bool out = false;
		if(p.x >= pmin.x && p.x <= pmax.x){
			if(p.y >= pmin.y && p.y <= pmax.y){
				out = true;
			}
		}
		
		return out;
	}
	
	bool AABB::CollideAABB(AABB other){
		bool out = true;
		
		for(int i = 0; i < 4; i++){
			if(!CollidePoint(other.boxv[i])){
				out = false;
				break;
			}
		}
		
		return out;
	}
}