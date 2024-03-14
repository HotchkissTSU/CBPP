/* 
Эта часть движка занимается всем, что связано с рендерингом

Отрисовка производится на своей скорости в своем потоке
 */
 
#ifndef CBPP_MAIN_THREADS
#define CBPP_MAIN_THREADS

#include "cb_main/settings.h"
#include "cbdraw/gpu_surf.h"
#include "cbdraw/window.h"

#include "SDL2/SDL.h"

#include <string>
#include <thread>
#include <chrono>

#include <iostream>

namespace cbutil{
	void RenderThreadFunc();
	void TickThreadFunc();
	
	struct RenderThread{ //да, чисто теоретически мы можем создать несколько потоков отрисовки, но никакой системы для
		void Run();      //синхронизации их работы я еще не придумал
		void Exit();
		
		RenderThread();
		
		void RenderThreadFunc();
		
		void SetMainDrawSurface(cbdraw::RenderSurface* rt);

		float update_rate, frame_time, fps_current;
		std::chrono::high_resolution_clock::time_point frame_start, frame_end;
		
		bool is_running = false, surf_allocated = false;
		
		cbdraw::RenderSurface* main_rsurf = nullptr;
		
		void (*OnRenderStart)(cbdraw::RenderSurface*) = nullptr;
		void (*BeforeRenderEnd)(cbdraw::RenderSurface*) = nullptr;
		
		//RenderThread() = delete;
		RenderThread(RenderThread const &) = delete;
		void operator=(RenderThread const &) = delete;
	};
}

#endif