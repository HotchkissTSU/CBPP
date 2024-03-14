#include "cb_main/render.h"

using namespace std;

namespace cbutil{	
	RenderThread::RenderThread(){
		update_rate = CBPP_FPS_LOCK;
		is_running = false;
	}
	
	void RenderThread::Run(){
		if(!is_running){
			std::thread thr(&RenderThreadFunc, this);
			thr.detach();
			
			is_running = true;
		}
	}
	
	void RenderThread::SetMainDrawSurface(cbdraw::RenderSurface* rt){
		if(rt != nullptr){
			main_rsurf = rt;
			surf_allocated = true;
		}
	}
	
	void RenderThread::RenderThreadFunc(){
		SDL_Renderer* rnd = cbutil::GetRender();
		SDL_Window* wnd = cbutil::GetWindow();
		
		while(is_running){
			frame_time = (int)(1000.0f/update_rate);
			
			frame_start = std::chrono::high_resolution_clock::now();
			SDL_Delay(frame_time);
			
			if(surf_allocated){
				main_rsurf->Clear();
				
				if(OnRenderStart != nullptr){
					OnRenderStart(main_rsurf);
				}
				
				if(BeforeRenderEnd != nullptr){
					BeforeRenderEnd(main_rsurf);
				}
				
				main_rsurf->Render(cbpp::Vec2(1));
			}
			
			frame_end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
			fps_current = (1000.0f/duration);
		}
	}
}