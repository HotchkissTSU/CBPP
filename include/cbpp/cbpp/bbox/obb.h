#ifndef CBPP_OBB
#define CBPP_OBB

#include "cbpp/bbox/aabb.h"
#include <vector>

namespace cbpp{
	extern "C" class OBB{
		public:
			OBB();
			//OBB(cbpp::AABB aabb, float angle);
			
			void UpdateAngle();
			
			void SetAngle(float ang);
			void SetAngle(int ang);
			float GetAngle();
			
			bool CollidePoint(Vec2 p);
			
		private:
			//cbpp::AABB raw_aabb;
			float ang;
	};
}

#endif