#ifndef CBPP_MAIN_DATA_H
#define CBPP_MAIN_DATA_H

#include <cstdint>
#include <cstddef>

#include "GLFW/glfw3.h"
#include "cbpp/event.h"

bool ModuleLoopCheck_default();

extern struct ModuleData_t {
	bool (*ModuleLoopCheck)( void ) = ModuleLoopCheck_default;
	bool (*ModuleMain)( int, char** ) = nullptr;
	void (*ModuleTick)( void ) = nullptr;
	void (*ModulePreMain)( void ) = nullptr;
	bool (*ModuleEventCallback) ( cbpp::Event& ) = nullptr;

	void* LibHandle = NULL;
	
} ModuleData;

extern struct GameData_t {
	constexpr static const char *DefaultWindowTitle = "CB++ Window", *DefaultGamefile = "assets/manifest/default.json";

	uint32_t WindowW = 800;
	uint32_t WindowH = 600;
	char* WindowTitle = NULL;

	GLFWwindow* MainWindow = NULL;
	GLFWmonitor* MainMonitor = NULL;

	char* Gamefile = NULL;
	char* GameLibrary = NULL;
} GameData;

#endif
