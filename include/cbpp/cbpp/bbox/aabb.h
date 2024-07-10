#ifndef CBPP_AABB
#define CBPP_AABB

#include <cstdint>
#include "cbpp/vec2.h"

/*
P0-------P1
 |       |
 |       |
 |       |
P2-------P3
*/

namespace cbpp{
	extern "C" struct AABB{
		AABB();
		AABB(const AABB& other);
		AABB(Vec2 minp, Vec2 maxp);
		
		void Set(Vec2 minp, Vec2 maxp);
		Vec2 operator[](uint8_t index);
		Vec2 At(uint8_t index);
		
		bool CollidePoint(Vec2 p);
		bool CollideAABB(AABB& other);
		
		void Allocate();
		void Free();
		
		~AABB();
		
		Vec2* pts = nullptr;
		bool allocated = false;
	};
}

#endif