#ifndef CBPP_BASE_ENTITY_H
#define CBPP_BASE_ENTITY_H

#define CB_ENTITY(ent_class_name) public:\
static BaseEntity* Create() {\
ent_class_name *new_ent = new ent_class_name;\
return dynamic_cast<BaseEntity*> (new_ent);}\

#include "cbpp/vec2.h"
#include "cbpp/utils.h"

namespace cbent {
	class BaseEntity {
		CB_VAR_GETSET(cbpp::Vec2, Pos, ent_pos)	
		CB_VAR_GETSET(float, Ang, ent_ang)

		CB_ENTITY(BaseEntity)

		//virtual void Think() = 0;
		//virtual void Render() = 0;

		public:
			BaseEntity() : ent_pos(cbpp::Vec2(0)), ent_ang(0.0f) {};
			BaseEntity( cbpp::Vec2 pos, float angle ) : ent_pos(pos), ent_ang(angle) {};
	};

	class TestEntity : public BaseEntity {
		CB_VAR_GETSET(int, TestInt, ent_test_int);

		CB_ENTITY(TestEntity)

		public:
			TestEntity() {};

			void SayHello() {
				printf("hello world!\n");
			}
	};
}

#endif