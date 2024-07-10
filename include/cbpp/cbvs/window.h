#ifndef CBPP_WINDOW_H
#define CBPP_WINDOW_H

#include "cbpp/vec2.h"

namespace cbpp {
	static struct BaseWindow {
		wchar_t* WindowTitle = nullptr;
		Vec2 WindowSize, WindowPos;
	};
	
	void InitMainWindow(wchar_t* title, Vec2 pos, Vec2 size){
		
	}
}

#endif