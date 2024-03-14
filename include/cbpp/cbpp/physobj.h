#ifndef CBPP_PHYSOBJECT
#define CBPP_PHYSOBJECT

#include "cbpp/vec2.h"
#include "cbpp/mesh.h"
#include <time.h>
#include <cmath>

#define CB_PHYSICS_VEL_DECCEL_KOEF 0.5f

namespace cbpp{	
	extern "C" struct PhysMaterial{
		PhysMaterial();
		
		float bounce,friction,ang_friction;
		float airdrag = 1;
	};
	
	struct BaseConstraint;
	
	extern "C" class PhysObj{
		public:
			PhysObj();
			PhysObj(Mesh* verts);
			PhysObj(Vec2 pos, Mesh* verts);
			PhysObj(Vec2 pos, float ang, Mesh* verts);
			
			void SetMesh(Mesh* verts);
			
			void UpdateDynamicMesh();
			
			int ConstraintTick();
			int Tick();		
			
			float GetKineticEnergy();
			
			void ForceCenter(Vec2 force);
			void ForceOffset(Vec2 force, Vec2 offset);
			
			void ForceAngular(float force);
			
			PhysObj* GetPhysObject();
			PhysMaterial* GetPhysMaterial();
			
			bool Valid();
			
			void AddConstraint(BaseConstraint* constr);
			void DeleteConstraint(BaseConstraint* constr);
			
			BaseConstraint* GetConstraintByID();
			unsigned GetIDByConstraint();
			
			unsigned GetEntityID();
			
			bool LockAngle=false, CanMove=true, IsSolid=true;
			Vec2 LinAccel, LinVel;
			float AngAccel = 0.0f, AngVel = 0.0f;
			int Mass = 1.0f;
			PhysMaterial PhysMat;
			float TickTime = 1.0f;
			
			unsigned ID = 0;
			
		private:
			Vec2 Pos;
			float Ang;
			
			int mass;
			Vec2 lin_accel, lin_vel;
			Vec2 fric_accel;
			float ang_accel, ang_vel;
			PhysMaterial pmat;
			
			Mesh* pobj_verts, pobj_verts_dynamic;
			
			bool move = true;
			bool solid = true;
			
			std::vector<BaseConstraint*> pobj_constr;
			std::vector<unsigned> pobj_nocollide;
			
			PhysObj* pobj_parent;
			std::vector<PhysObj*> pobj_children;
			
			unsigned pobj_ent_id;

			float tick_time;
	};
	
	extern "C" struct PhysEnv{
		PhysEnv();
		
		void Tick();
		
		std::vector<PhysObj> pobj_list;
	};
	
	extern "C" struct BaseConstraint{
		BaseConstraint(PhysObj& pobj1, PhysObj& pobj2);
		
		void Tick();
		
		PhysObj& o1, o2;
		unsigned constr_id = 0;
		bool rigid = false;
		
		float TickTime;
	};
	
	void AddConstraint(BaseConstraint* constr);
}

#endif