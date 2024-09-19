#ifndef CBPP_GAME_INTERFACE_H
#define CBPP_GAME_INTERFACE_H

#include <cstdint>
#include "cbpp/vec2.h"

enum CBPP_MODULE_TYPE : uint8_t {
	MODULE_NONE,
	MODULE_GAME,
	MODULE_EDITOR
};

struct CBPP_ModuleInfo {
	const char* ModuleName = nullptr;
	CBPP_MODULE_TYPE ModuleType = MODULE_NONE;
	
	cbpp::Vec2 WindowPos, WindowSize;
	const char* WindowTitle = "WINDOW";
	bool WindowFullscreen = false;
};

#endif