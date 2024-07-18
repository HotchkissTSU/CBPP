#include "cbpp/entity/base_entity.h"

namespace cbent {
	cbpp::SegArray<Entity*>& GetEntityList(){
		return entlist;
	}
	
	void TestComponent::Print(){
		std::cout<<"TEST COMPONENT\n";
	}
}