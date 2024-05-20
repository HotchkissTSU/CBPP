#include "cbpp.h"
#include <iostream>
#include <stdio.h>

#include <cmath>

using namespace cbpp;
using namespace cbdraw;

void test_sfx(int chan, void *stream, int len, void *udata){
	uint8_t* int_stream = (uint8_t*)stream;
	
	for(std::size_t i = 1; i < len; i++){
		int_stream[i] = (uint8_t)int_stream[i];
	}
}

int test_hook(){
	std::cout<<"pre-init hook called!\n";
	
	return 228;
}

#ifdef main
#undef main
#endif

int main(){	
	cbutil::Init("nigger", cbpp::Vec2(100), cbpp::Vec2(1000,600), SDL_WINDOW_SHOWN);
	
	SDL_Renderer* rnd = cbutil::GetRender();
	SDL_Window* wnd = cbutil::GetWindow();

	cbdraw::RenderSurface* cam = new cbdraw::RenderSurface(cbpp::Vec2(), cbpp::Vec2(500), rnd);

	bool run = true;
	
	uint64_t c = 0;
	
	int mx,my;
	bool m1;
	
	while(run){
		c++;
		SDL_Delay(5);
		
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
			switch (ev.type){
				case SDL_QUIT:
					run = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					m1 = true;
					break;
				case SDL_MOUSEBUTTONUP:
					m1 = false;
					break;
			}
		}
		
		SDL_GetMouseState(&mx, &my);	
		
		cam->Clear();

		cam->Render(rnd, Vec2(1000,600));
	}
	
	delete cam;
	
	return 0;
}
