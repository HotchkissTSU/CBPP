#include "cb_main/interface.h"
#include "cbpp/statereg.h"

#include <iostream>

const CBPP_ModuleInfo _INFO {
	"TemplateModule",
	MODULE_GAME,
	cbpp::Vec2(200), cbpp::Vec2(800,600), "Test Window", false
};

bool TestTick(){
	std::cout<<"TEST TICK\n";
	
	return true;
}

bool TestEvent(event_t& ev){
	return true;
}

extern "C" {
	const CBPP_ModuleInfo& CBPP_GetModuleInfo(){
		return _INFO;
	}
	
	bool CBPP_ModuleMain(int argc, char* argv[]){
		std::cout<<"MODULE MAIN FUNC\n";
		
		cbpp::GameState* test_gs = cbpp::CreateGameState("TestState", &TestTick, &TestEvent);
		uint32_t test_gs_id = cbpp::RegisterGameState(test_gs);
		
		cbpp::SetGameState(test_gs_id);
		
		return true;
	}
	
	bool CBPP_MainLoopCheck(){
		return true;
	}
}