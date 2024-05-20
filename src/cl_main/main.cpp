#include "cbpp.h"

#include "cb_main/render.h"
#include "cb_main/settings.h"

#include <iostream>

bool enable_video = true;
bool gamefile_providen = false;

int TPS_LOCK = CBPP_TPS_LOCK;
int FPS_LOCK = CBPP_FPS_LOCK;
cbpp::Vec2 RESOLUTION(800, 600);

cbpp::Log boot_log("startup");

using namespace cbpp;

Mesh* MESH = new Mesh(3);
Mesh* CONVEX = new Mesh(3);

void Initialize(std::string window_title, cbpp::Vec2 window_pos, cbpp::Vec2 window_size, SDL_WindowFlags window_flags);
void ParseArgs(int argc, char* argv[]);
void TickThreadFunc(float tps_lock);
void ParseGameFile(std::string fname);

std::wstring FloatToWString(float v){
	wchar_t buff[10];
	
	_itow((int)v, buff, 10);
	
	return std::wstring(buff);
}

bool file_validate(std::string file){
	std::ifstream test(file);
	bool out = test.is_open();
	test.close();
	
	return out;
}

void pre_rnd_hook(cbdraw::RenderSurface* rt){
	
	//for(int i = 0; i < MESH->Size(); i++){
		//rt->DrawCircle(cbdraw::Color(255,255,255,255), MESH->At(i), 3, 2);
	//}
	
	//rt->DrawMesh(cbdraw::Color(255,255, 0 ,100), Vec2(0), CONVEX, 5);
	
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	
	mCircle c1;
	
	c1.pos = Vec2(400);
	c1.radius = 100.0f;
	
	mCircle c2;
	c2.pos = Vec2(mx,my);
	c2.radius = 50.0f;
	
	rt->DrawCircle(cbdraw::Color(255,255,255,255), c1.pos, c1.radius, 2);	
	rt->DrawCircle(cbdraw::Color(255,255,255,255), c2.pos, c2.radius, 2);
	
	Intersection isec = Intersect_CircleCircle(c1,c2);
	
	for(int i = 0; i < isec.points.size(); i++){
		rt->DrawCircle(cbdraw::Color(255,0,0,255), isec.points[i], 3, 2);
	}
}

int main(int argc, char* argv[]){
	
	MESH->At(0) = Vec2(100,100);
	MESH->At(1) = Vec2(150,100);
	MESH->At(2) = Vec2(160,100);
	
	ParseArgs(argc, argv);
	
	Initialize("Cuber++", cbpp::Vec2(100), RESOLUTION, SDL_WINDOW_SHOWN);
	
	cbdraw::RenderSurface* main_cam = nullptr;
	
	if(enable_video){
		boot_log.WriteInfo(L"RenderThread FPS lock: "+FloatToWString(FPS_LOCK));
		
		auto renderer = cbutil::RenderThread();
		
		main_cam = new cbdraw::RenderSurface(cbpp::Vec2(), RESOLUTION, cbutil::GetRender());
		renderer.SetMainDrawSurface(main_cam);
		
		renderer.OnRenderStart = &pre_rnd_hook;
		
		renderer.Run();
	}else{
		boot_log.WriteInfo(L"Rendering is disabled for this instance");
	}
	
	boot_log.WriteInfo(L"MainThread TPS lock: " + FloatToWString(TPS_LOCK));
	
	TickThreadFunc(TPS_LOCK);
	
	boot_log.Close();
	
	if(main_cam != nullptr){
		delete main_cam;
	}
	
	return 0;
}

void Initialize(std::string window_title, cbpp::Vec2 window_pos, cbpp::Vec2 window_size, SDL_WindowFlags window_flags){
	cbutil::Init(window_title, window_pos, window_size, window_flags);
}

void ParseArgs(int argc, char* argv[]){
	for(uint16_t i = 0; i < argc; i++){
		std::string arg = std::string(argv[i]);
		
		if(arg == "-novideo"){
			enable_video = false;
		}
		
		if(arg == "-game"){
			int a;
			if(i < argc-1){
				std::string next(argv[i+1]);
				if(file_validate(next)){
					gamefile_providen = true;
					
					ParseGameFile(next);
				}else{
					std::cout<<"Gamefile \""<<next<<"\" does not exist"<<std::endl;
					scanf("%i", &a);
					exit(1);
				}
			}else{
				std::cout<<"No gamefile is specified"<<std::endl;
				scanf("%i", &a);
				exit(1);
			}
		}
	}
}

void TickThreadFunc(float tps_lock = 0){
	if(tps_lock == 0){
		tps_lock = CBPP_TPS_LOCK;
	}
	
	//cbsv::ConnectToHost("localhost", 16000);
	
	bool is_running = true;
	
	float update_rate, frame_time, tps_current;
	std::chrono::high_resolution_clock::time_point frame_start, frame_end;
	
	update_rate = tps_lock;
	
	int mx,my;
	
	while(is_running){
		frame_time = (int)(1000.0f/update_rate);
		
		frame_start = std::chrono::high_resolution_clock::now();
		
		SDL_Delay(frame_time);
		
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
			switch (ev.type){
				case SDL_QUIT:
					is_running = false;
					
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mx, &my);
					
					MESH->AdjustVertex(Vec2(mx,my));
					
					MESH->GetConvex(CONVEX);
				
			}
		}
		
		frame_end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
		tps_current = (1000.0f/duration);
	}
}

void ParseGameFile(std::string fname){
	std::ifstream _if(fname);
	
	if(!_if.is_open()){ gamefile_providen = false; return; }
	
	Json::Reader reader;
	Json::Value root;
	
	if(!reader.parse(_if, root)) {
		std::cout << reader.getFormattedErrorMessages() << std::endl;
		
		int a;
		scanf("%i", &a);
		exit(1);
	}
}

