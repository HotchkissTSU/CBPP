#include "cbpp/hitbox.h"
#include "cbpp/cb_alloc.h"

namespace cbpp{ //BaseHitbox
	BaseHitbox::BaseHitbox(CBPP_HBTYPE hbt){ hb_type = hbt; }
	
	CBPP_HBTYPE BaseHitbox::GetClass(){ return hb_type; }
	
	AABB BaseHitbox::GetAABB(){ return hb_aabb; }
	
	void BaseHitbox::UpdateAABB(){}
	
	HitBoxCollision BaseHitbox::CollidesWith(BaseHitbox& other){}
	
	BaseHitbox::~BaseHitbox(){}
}

namespace cbpp{ //MeshHitbox
	MeshHitbox::MeshHitbox(Mesh hb_mesh) : BaseHitbox(HITBOX_MESH) {
		hb_static = hb_mesh;
		hb_dynamic = hb_mesh;
		
		UpdateAABB();
	}
	
	void MeshHitbox::UpdateAABB(){
		float min_x = hb_dynamic[0].x, min_y = hb_dynamic[0].y;
		float max_x = hb_dynamic[0].x, max_y = hb_dynamic[0].y;
		
		for(uint32_t i = 0; i < hb_dynamic.Size(); i++){
			Vec2 pt = hb_dynamic[i];
			if(pt.x > max_x){ max_x = pt.x; }
			if(pt.x < min_x){ min_x = pt.x; }
			
			if(pt.y > max_y){ max_y = pt.y; }
			if(pt.y < min_y){ min_y = pt.y; }
		}
		
		hb_aabb.Set( Vec2(min_x, min_y), Vec2(max_x, max_y) );
	}
	
	void MeshHitbox::SetAngle(float flt){
		hb_static = hb_dynamic.GetRotatedByCenter(flt);
		UpdateAABB();
	}
	
	float MeshHitbox::GetAngle(){ return hb_angle; }
	
	Mesh MeshHitbox::GetDynamicMesh(){ return hb_dynamic; }
	Mesh MeshHitbox::GetStaticMesh(){ return hb_static; }
}

namespace cbpp{ //Collision detection functions
	HitBoxCollision Coll_MeshCircle(MeshHitbox& a, CircleHitbox& b){
		HitBoxCollision out;
		
		
	}
}