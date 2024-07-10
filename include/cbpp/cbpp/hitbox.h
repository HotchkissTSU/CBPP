#ifndef CBPP_HITBOXH
#define CBPP_HITBOXH

#include <cstdint>

#include "cbpp/vec2.h"
#include "cbpp/mesh.h"
#include "cbpp/bbox/aabb.h"

namespace cbpp{
	enum CBPP_HBTYPE : uint8_t {
		HITBOX_UNDEF,
		
		HITBOX_MESH,
		HITBOX_CIRCLE
	};
	
	enum CBPP_COLLCLASS : uint8_t {
		COLLIDE_PHYS =   0b00000001, //другие COLLIDE_PHYS
		COLLIDE_BULLET = 0b00000010, //проджектайлы
		COLLIDE_TRACE =  0b00000100, //трейсы
		COLLIDE_HUMAN =  0b00001000, //игроки / боты
		COLLIDE_LOS =    0b00010000, //блокирует линию взгляда
		COLLIDE_NAV =    0b00100000, //блокировка соединения узлов нав. графа при компиляции карты
		
		COLLIDE_GROUPA = 0b01000000,
		COLLIDE_GROUPB = 0b10000000, //две абстрактные группы (я не придумал, куда девать ещё два бита)
		
		COLLIDE_ALL =    0b11111111, //блокирует всё
		COLLIDE_NONE =   0b00000000  //эфемерный
	};
	
	struct HitBoxCollision {
		bool Collide = false;
		float CollisionDepth = 0.0f;
		float CollisionSurface = 0.0f;
		
		Vec2 CollisionNormal, CollisionPos;
	};
	
	extern "C" class BaseHitbox{
		public:
			BaseHitbox(CBPP_HBTYPE hb_class);
			
			CBPP_HBTYPE GetClass();
			AABB GetAABB();
			
			virtual HitBoxCollision CollidesWith(BaseHitbox& other);
			
			virtual void UpdateAABB();
			
			virtual ~BaseHitbox();
		
		protected:
			CBPP_HBTYPE hb_type = HITBOX_UNDEF;
			AABB hb_aabb;
	};
	
	extern "C" class MeshHitbox : BaseHitbox {
		public:
			MeshHitbox(Mesh hb_msh);
			
			void UpdateAABB();
			
			void SetAngle(float nang);
			float GetAngle();
			
			Mesh GetDynamicMesh();
			Mesh GetStaticMesh();
		
		private:
			Mesh hb_static, hb_dynamic;
			float hb_angle = 0.0f;
	};
	
	extern "C" class CircleHitbox : BaseHitbox {
		public:
			CircleHitbox(float radius);
			
			void SetRadius(float nrad);
			float GetRadius();
			
			void UpdateAABB();
			
		private:
			float hb_radius = 1.0f;
	};
}

namespace cbpp{ //функции-коллайдеры
	extern "C" HitBoxCollision Coll_MeshMesh(MeshHitbox& a, MeshHitbox& b);
	extern "C" HitBoxCollision Coll_MeshCircle(MeshHitbox& a, CircleHitbox& b);
	extern "C" HitBoxCollision Coll_MeshAABB(MeshHitbox& a, AABB b);
	extern "C" HitBoxCollision Coll_MeshPoint(MeshHitbox& a, Vec2 b);
	
	extern "C" HitBoxCollision Coll_CircleCircle(CircleHitbox& a, CircleHitbox& b);
	extern "C" HitBoxCollision Coll_CirclePoint(CircleHitbox& a, Vec2 b);
	extern "C" HitBoxCollision Coll_CircleAABB(CircleHitbox& a, AABB b);
}

namespace cbpp{
	extern "C" class HitboxGroup{
		public:
			HitboxGroup();
			HitboxGroup(HitboxGroup& hbref);
			
		private:
			BaseHitbox* hbarr = nullptr;
			uint16_t arrlen = 0;
	};
}

#endif