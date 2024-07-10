#include "cbdraw/window.h"
#include <math.h>

#include <iostream>

namespace cbutil{
	int _default_hook(){ return 0; }
	
	HookManager::Hook(){
		hookmap.clear();
	}
	
	void HookManager::SetHook(std::string name, int (*hook_f)()){
		
		if(hook_f != nullptr){
			std::size_t val = (std::size_t)(void*)hook_f;
			
			hookmap[name] = val;
		}else{
			hookmap[name] = (std::size_t)(void*)_default_hook;
		}
	}
	
	int HookManager::CallHook(std::string name){
		if(hookmap.find(name) != hookmap.end()){
			if(hookmap[name] != 0){
				int (*foo)() = (int(*)())((void*)hookmap.at(name));
				return foo();
			}
		}else{
			return 0;
		}
		
		return 0;
	}
	
	void HookManager::ResetHook(std::string name){
		hookmap[name] = (std::size_t)(void*)_default_hook;
	}
}

namespace cbutil{
	SDL_Renderer* GetRender(){ return w_render; }
	SDL_Window* GetWindow(){ return w_window; }

	int _baseEngineInit(std::string title, cbpp::Vec2 pos, cbpp::Vec2 size, SDL_WindowFlags flags){
		int code = 1;
		
		code = code && SDL_Init(SDL_INIT_EVERYTHING);
		code = code && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		code = code && TTF_Init();
		
		w_window = SDL_CreateWindow(title.c_str(), std::abs((int)pos.x), std::abs((int)pos.y), std::abs((int)size.x), std::abs((int)size.y), flags);
		w_render = SDL_CreateRenderer(w_window, -1, SDL_RENDERER_TARGETTEXTURE);

		return code && (w_window != nullptr) && (w_render != nullptr);
	}

	int Init(std::string window_title, cbpp::Vec2 window_pos, cbpp::Vec2 window_size, SDL_WindowFlags window_flags){
		int ret_code = 1;
		
		_baseEngineInit(window_title, window_pos, window_size, window_flags);
		
		return ret_code;
	}
	
	void InitHook(){
		Hook = HookManager();
	}
}