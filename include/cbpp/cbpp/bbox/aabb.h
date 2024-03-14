#ifndef CBPP_AABB
#define CBPP_AABB

#include "cbpp/vec2.h"
#include "cbpp/bbox/obb.h"
#include <vector>

namespace cbpp{
	extern "C" class AABB{
		public:
			AABB(Vec2 minp, Vec2 maxp);
			AABB();
			
			bool CollidePoint(Vec2 p);
			bool CollideAABB(cbpp::AABB other);
			
			void Set(Vec2 minp, Vec2 maxp);
			void SetMin(Vec2 minp);
			void SetMax(Vec2 maxp);
			
			Vec2 GetMin();
			Vec2 GetMax();
			
			std::vector<Vec2> GetBoxVector();
			void GenerateBoxVector();
			
		private:
			Vec2 pmin, pmax;
			std::vector<Vec2> boxv = std::vector<Vec2>(4);
	};
}

#endif