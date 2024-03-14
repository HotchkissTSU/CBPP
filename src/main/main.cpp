#include "cbpp.h"

#include "cb_main/render.h"

#include <iostream>

bool enable_video = true;
bool gamefile_providen = false;

int TPS_LOCK = CBPP_TPS_LOCK;
int FPS_LOCK = CBPP_FPS_LOCK;
cbpp::Vec2 RESOLUTION(800, 600);

cbpp::Log boot_log("startup");

using namespace cbpp;

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
	rt->DrawCircle(cbdraw::Color(255,0,0,255), cbpp::Vec2(100), 100, 3);
}

int main(int argc, char* argv[]){
	
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
	
	bool is_running = true;
	
	float update_rate, frame_time, tps_current;
	std::chrono::high_resolution_clock::time_point frame_start, frame_end;
	
	update_rate = tps_lock;
	
	//cbsv::RunServer(3000);
	
	//std::cout<<cbsv::server_is_run<<std::endl;
	
	cbpp::ByteTable test;
	
	test.WriteValueSingle<int32_t>("test_int", 1024, INT32);
	test.WriteValueSingle<cbpp::Vec2>("test_vector", cbpp::Vec2(-1.0f, 45.67f), VEC2);
	
	//std::cout<<test.ReadValueSingle<int32_t>("test_int")<<std::endl;
	
	test.SetHeader("nigger");
	auto sarr = test.Assemble();
	
	//sarr.PrintVerbose<int>(8);
	
	test.Disassemble(sarr);
	
	std::cout<<test.ReadValueSingle<int32_t>("test_int")<<std::endl;
	std::cout<<test.ReadValueSingle<cbpp::Vec2>("test_vector")<<std::endl;
	
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

