#ifndef CBDRAW_WINDOWH
#define CBDRAW_WINDOWH

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_ttf.h"
#include "SDL2/SDL_net.h"

#include "cbpp/cbpp.h"
#include <string>

#include <map>

namespace cbutil{
	int _default_hook();
	
	extern "C" class HookManager{
		public:
			Hook();
		
			bool is_init = false;
		
			std::map< std::string, std::size_t > hookmap;
			
			void SetHook(std::string name, int (*hook_f)());
			int CallHook(std::string name);
			void ResetHook(std::string name);
	};
	
	static SDL_Renderer* w_render;
	static SDL_Window* w_window;
	
	static HookManager Hook;
	
	SDL_Renderer* GetRender();
	SDL_Window* GetWindow();
	
	int CallHook(int (*_hook)());
	
	int Init(std::string window_title, cbpp::Vec2 window_pos, cbpp::Vec2 window_size, SDL_WindowFlags window_flags);
	void InitHook();
}

#endif