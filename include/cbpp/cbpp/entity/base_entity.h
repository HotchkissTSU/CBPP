#ifndef CBPP_BASE_ENTITY_H
#define CBPP_BASE_ENTITY_H

#include "cbpp/vec2.h"
#include "cbpp/ttype/segarr.h"

#include <cstdint>
#include <map>
#include <string>
#include <iostream>

#define CBPP_COMPONENT_ID_OFFSET 1500

#define COMPONENT_ID(__ent_id)\
	public: static EntComponentID GetID(){\
		return __ent_id;\
	}
	
#define CBPP_COMPONENT_ID(__ent_id)\
	public: static EntComponentID GetID(){\
		return __ent_id + CBPP_COMPONENT_ID_OFFSET;\
	}
	
#define COMPONENT_ID_AUTO() COMPONENT_ID(__COUNTER__)
#define CBPP_COMPONENT_ID_AUTO() CBPP_COMPONENT_ID(__COUNTER__)

namespace cbent {
	typedef uint32_t EntComponentID;
	typedef uint32_t EntityID;
	
	enum ENTITY_SYNCTYPE : uint8_t {
		SYNC_NONE,      //никакой синхронизации
		SYNC_ALWAYS,    //постоянная синхронизация
		SYNC_MKDEL,     //синхронизация создания и удаления сущности
		SYNC_PROXIMITY, //синхронизация тогда, когда сущность посчитает себя достаточно близко
		SYNC_MANUAL     //только по явно поданной команде
	};
	
	class Entity;

	class BaseComponent {
		public:
			CBPP_COMPONENT_ID(1)
			
			Entity* Owner = nullptr;
	};
	
	class TestComponent : public BaseComponent {
		public:			
			CBPP_COMPONENT_ID(2)
			
			void Print();
	};
	
	class GimbalComponent : public BaseComponent {
		public:
			CBPP_COMPONENT_ID(3)
			
			cbpp::Vec2 Position;
			float Angle = 0.0f;
	};
	
	class DevTestVisualComponent : public BaseComponent {
		public:
			CBPP_COMPONENT_ID(4)
			
			void Render();
			
			float Size = 35.0f;
	};
	
	class Entity {
		public:
			template<class T> void CreateComponent(){
				T* new_comp = new T;
				new_comp->Owner = this;
				comps[T::GetID()] = new_comp;
			}
			
			template<class T> T* GetComponent(){
				EntComponentID comp_name = T::GetID();
				if(comps.find(comp_name) == comps.end()){
					return nullptr;
				}
				
				return (T*)(comps[comp_name]);
			}
			
		private:			
			std::map<EntComponentID, BaseComponent*> comps;
	};
	
	static cbpp::SegArray<Entity*> entlist(1024, 256);
	cbpp::SegArray<Entity*>& GetEntityList();
	
	EntityID SpawnEntity(Entity* ptr);
}

#endif