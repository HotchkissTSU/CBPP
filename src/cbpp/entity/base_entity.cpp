#include "cbpp/entity/base_entity.h"

using namespace cbpp;

namespace cbent {
	EntityID SpawnEntity(Entity* ptr){
		if(ptr == nullptr){
			return 0;
		}
		
		EntityID place_index = entlist.Emplace(ptr);
		return place_index;
	}
	
	SegArray<Entity*>& GetEntityList(){
		return entlist;
	}
	
	void TestComponent::Print(){
		std::cout<<"TEST COMPONENT\n";
	}
}