#ifndef CBPP_BASE_ENTITY_H
#define CBPP_BASE_ENTITY_H

#include "cbpp/vec2.h"
#include "cbpp/cbdef.h"
#include "cbpp/ai.h"
#include "cbpp/cb_hash.h"
#include "cbpp/cbstring.h"

#include <map>
#include <queue>

#define ENTITY_COMPONENT(trivial_name) private:\
static constexpr cbpp::hash_t comp_uid = cbpp::Hash(#trivial_name);\
public:\
static cbpp::hash_t GetID() { return comp_uid; }\
static const char* GetName() { return #trivial_name; }

namespace cbent {
	class Entity;

	class BaseComponent {
		ENTITY_COMPONENT(component_base);
		CB_VAR_GET(const Entity*, Owner, comp_owner);

		public:
			BaseComponent() = default;
			BaseComponent(Entity* owner) {
				comp_owner = const_cast<const Entity*> (owner);
			}
	};

	class GimbalComponent : public BaseComponent {
		ENTITY_COMPONENT(gimbal_component);
		CB_VAR_GETSET(cbpp::Vec2, Pos, ent_pos);
		CB_VAR_GETSET(float, Ang, ent_angle);

		public:
			GimbalComponent() = default;
			GimbalComponent(Entity* owner) : BaseComponent(owner) {};
	};

	class NetworkableComponent : public BaseComponent {
		ENTITY_COMPONENT(networkable_component);

		public:
			NetworkableComponent() = default;
			NetworkableComponent(Entity* owner) : BaseComponent(owner) {};
	};

	class AIComponent : public BaseComponent {
		ENTITY_COMPONENT(ai_component);

		public:
			AIComponent() = default;
			AIComponent(Entity* owner) : BaseComponent(owner) {};

			void PushTask(cbpp::AI_Task& task);

		private:
			std::priority_queue<cbpp::AI_Task, std::vector<cbpp::AI_Task>, std::less<cbpp::AI_Task>> comp_tasks;
	};

	class CollisionComponent : public BaseComponent {
		ENTITY_COMPONENT(collision_component);

		public:
			CollisionComponent() = default;
			CollisionComponent(Entity* owner) : BaseComponent(owner) {};
	};

	template<class T> BaseComponent* CreateComponent(Entity* owner) {
		return dynamic_cast<BaseComponent*>(new T(owner));
	}

	class Entity {
		CB_VAR_GET(uint64_t, ID, ent_id);
		CB_VAR_GETSET(cbpp::String, Name, ent_name);

		public:
			Entity() = default;

			template<class T> void AttachComponent() {
				cbpp::hash_t comp_id = T::ID();

				if(ent_comps.count(comp_id) <= 0) {
					ent_comps[comp_id] = CreateComponent<T>(this);
				}
			}

			template<class T> T* GetComponent() {
				const char* comp_name = T::GetName();
				cbpp::hash_t comp_hash = cbpp::Hash(comp_name);
				
				if(ent_comps.count(comp_hash) > 0) {
					return (T*)( ent_comps.at(comp_hash) );
				}else {
					return NULL;
				}
			}

		private:
			std::map<cbpp::hash_t, BaseComponent*> ent_comps;
	};
}

#endif