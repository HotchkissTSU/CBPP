#ifndef CBPP_BASE_ENTITY_H
#define CBPP_BASE_ENTITY_H

#include "cbpp/vec2.h"
#include "cbpp/ttype/segarr.h"

#include <cstdint>
#include <map>
#include <string>
#include <iostream>

namespace cbent {
	typedef std::string EntComponentID;
	
	template<class T> EntComponentID GetComponentName(){
		return std::string( typeid(T).name() );
	}
	
	enum ENTITY_SYNCTYPE : uint8_t {
		SYNC_NONE,      //никакой синхронизации
		SYNC_ALWAYS,    //постоянная синхронизация
		SYNC_MKDEL,     //синхронизация создания и удаления сущности
		SYNC_PROXIMITY, //синхронизация тогда, когда сущность посчитает себя достаточно близко
		SYNC_MANUAL     //только по явно поданной команде
	};

	class BaseComponent {
		public:
			const EntComponentID ID = GetComponentName<BaseComponent>();
	};
	
	class TestComponent : public BaseComponent {
		public:			
			const EntComponentID ID = GetComponentName<TestComponent>();
			
			void Print();
	};
	
	class Entity {
		public:
			template<class T> void CreateComponent(){
				T* new_comp = new T;
				std::cout<<new_comp<<std::endl;
				comps[new_comp->ID] = new_comp;
			}
			
			template<class T> T* GetComponent(){
				EntComponentID comp_name = GetComponentName<T>();
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
}

#endif