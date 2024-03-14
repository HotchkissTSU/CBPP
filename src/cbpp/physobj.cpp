#include "cbpp/physobj.h"

namespace cbpp{ //physics object
	Vec2 ease2zero(Vec2 val, float spd){
		Vec2 tmp = val + ClampVector(val*-1, -spd, spd);
		return tmp;
	}

	void PhysObj::SetMesh(Mesh* verts){
		for(std::size_t i = 0; i < verts->Size(); i++){
			pobj_verts->AdjustVertex(verts->At(i));
			pobj_verts->AdjustVertex(verts->At(i));
		}
		
		Pos = Vec2();
		Ang = 0.0f;
	}
	
	void PhysObj::UpdateDynamicMesh(){
		Mesh* tmp(pobj_verts);
		
		for(std::size_t i = 0; i < pobj_verts->Size(); i++){
			pobj_verts_dynamic[i] = pobj_verts->At(i).GetRotated(Ang);
		}
	}
	
	PhysObj::PhysObj(){}
	
	PhysObj::PhysObj(Mesh* verts){
		SetMesh(verts);
	}
	
	PhysObj::PhysObj(Vec2 pos, Mesh* verts){
		Pos = pos;
		SetMesh(verts);
	}
	
	PhysObj::PhysObj(Vec2 pos, float ang, Mesh* verts){
		Pos = pos;
		Ang = ang;
		SetMesh(verts);
	}
	
	int PhysObj::ConstraintTick(){
		for(std::size_t i = 0; i < pobj_constr.size(); i++){
			BaseConstraint* constr = pobj_constr[i];
			constr->Tick();
		}
		
		return 0; 
	}
	
	int PhysObj::Tick(){		
		LinVel = LinVel + LinAccel*TickTime;
		
		if(PhysMat.friction != 0.0f){
			LinAccel = LinAccel - LinVel.Norm()*Mass*TickTime*PhysMat.friction;
			LinAccel = ease2zero(LinAccel, PhysMat.friction*TickTime*Mass); //это вообще не по физике, но это работает
			LinVel = ease2zero(LinVel, PhysMat.friction*TickTime*Mass*CB_PHYSICS_VEL_DECCEL_KOEF);
		}
		
		if(move){
			Pos = Pos + LinVel*TickTime;
		}
		
		return 0;
	}
	
	float PhysObj::GetKineticEnergy(){
		float scal_vel = LinVel.Length();
		return (float)(Mass*scal_vel*scal_vel*0.5f);
	}
	
	void PhysObj::ForceCenter(Vec2 force){ //F = m*a, a = F/m
		if(Mass == 0){
			LinAccel = 0;
		}else{
			if(force.Length() != 0.0f){
				LinAccel = LinAccel + force/Mass;
			}
		}
	}
	
	void PhysObj::ForceOffset(Vec2 force, Vec2 offset){
		Vec2 hand = Pos - offset;
		
		float hand_length = hand.Length();
		
		
	}
	
	void PhysObj::ForceAngular(float force){
		if(Mass == 0){ AngAccel = 0; }
		else{ AngAccel = force/Mass; }
	}
	
	bool PhysObj::Valid(){
		return pobj_verts->Size() >= 3; 
	}
	
	//constraint system ----------------------------
	void PhysObj::AddConstraint(BaseConstraint* constr){
		pobj_constr.push_back(constr);
	}
	
	void AddConstraint(BaseConstraint* constr){
		if(constr->o1.Valid() && constr->o2.Valid()){
			constr->o1.AddConstraint(constr);
			constr->o2.AddConstraint(constr);
		}
	}
}

namespace cbpp{ //physics material and BaseConstraint
	PhysMaterial::PhysMaterial(){}
	
	void BaseConstraint::Tick(){
		//base constraint does nothing
	}
}