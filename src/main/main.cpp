#include "cbpp.h"

#include "json/json.h"
#include "png/png.h"

#include "cb_main/error.h"
#include "cb_main/interface.h"
#include "cb_main/settings.h"
#include "cb_main/glfw_callbacks.h"

#include <windows.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <stdio.h>

using namespace cbpp;

const CBPP_ModuleInfo& (*CBPP_GetModuleInfo)( void ) = nullptr;
bool (*CBPP_ModuleMain)( int argc, char* argv[] ) = nullptr;
bool (*CBPP_MainLoopCheck)( void ) = nullptr;

GLFWwindow* CBPP_MainWindow = nullptr;

float CBPP_tick_timing = 0.1f;
clock_t CBPP_tick_start, CBPP_tick_end, CBPP_tick_delta;

CBPP_ModuleInfo CBPP_CurrentModuleInfo;

const char* CBPP_GameLibrary = nullptr;

HMODULE CBPP_ModuleLibHandle;

GLuint vao = 0, vbo = 0;
cbdraw::Shader* shader_test;

LONG WINAPI CBPP_ExceptHandle(PEXCEPTION_POINTERS exception){
	EXCEPTION_RECORD* rec = exception->ExceptionRecord;
	std::string text;
	
	switch(rec->ExceptionCode){
		case EXCEPTION_ACCESS_VIOLATION:
			text += "EXCEPTION_ACCESS_VIOLATION\n";
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			text += "EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n";
			break;
		case EXCEPTION_BREAKPOINT:
			text += "EXCEPTION_BREAKPOINT\n";
			break;
	}
	
	std::size_t exc_addr = (std::size_t)(rec->ExceptionAddress);
	char addr_text[20];
	itoa(exc_addr, addr_text, 16);
	
	text += "Exception address: ";
	text += addr_text;
	text += '\n';
	
	int numparams = rec->NumberParameters;
	if(numparams){
		text += "\nException information:\n";
		
		if(rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION){
			switch(rec->ExceptionInformation[0]){
				case 1:
					text += "\tAttempt to write in an inaccessible address\n";
					break;
				case 0:
					text += "\tAttempt to read from the inaccessible data\n";
					break;
				case 8:
					text += "\tDEP violation\n";
					break;
			}
		}
	}
	
	text += "\nThe engine will crash right now.\n";
	
	cbpp::DisplayError("Lethal exception in CBPP", text.c_str(), true);
	
	return EXCEPTION_EXECUTE_HANDLER;
}

void LoadGameLibrary(){
	CBPP_ModuleLibHandle = LoadLibrary(CBPP_GameLibrary);
	
	if(CBPP_ModuleLibHandle == NULL){
		cbpp::DisplayError("ERROR", (std::string("Unable to load the specified module: \n") + std::string(CBPP_GameLibrary)).c_str(), true);
	}
	
	CBPP_GetModuleInfo = (const CBPP_ModuleInfo& (*)( void ))GetProcAddress(CBPP_ModuleLibHandle, CBPP_GETFUNKNAME);
	CBPP_ModuleMain = (bool (*)(int, char**))GetProcAddress(CBPP_ModuleLibHandle, CBPP_MAINFUNCNAME);
	CBPP_MainLoopCheck = (bool (*)(void))GetProcAddress(CBPP_ModuleLibHandle, CBPP_LOOPCHECKFUNCNAME);
	
	if(CBPP_GetModuleInfo == NULL){
		cbpp::DisplayError("ERROR", "GetProcAddress for CBPP_GetModuleInfo returned NULL", true);
	}
	
	if(CBPP_ModuleMain == NULL){
		cbpp::DisplayError("ERROR", "GetProcAddress for CBPP_ModuleMain returned NULL", true);
	}
	
	if(CBPP_MainLoopCheck == NULL){
		cbpp::DisplayError("ERROR", "GetProcAddress for CBPP_MainLoopCheck returned NULL", true);
	}
	
	CBPP_CurrentModuleInfo = CBPP_GetModuleInfo();
}

void ParseGameFile(){
	Json::Reader rd;
	Json::Value root;
	
	std::ifstream json_in(CBPP_GAMEFILE_NAME);
	
	if(!json_in.is_open()){
		cbpp::DisplayError("ERROR", "Unable to locate game.json", true);
	}
	
	if(!rd.parse(json_in, root)) {		
		cbpp::DisplayError("Gamefile parsing error", rd.getFormattedErrorMessages().c_str(), true);
	}
	
	json_in.close();
	
	CBPP_GameLibrary = root["base"].asString().c_str();
}

void CBPP_CreateWindow(){
	int W = (int)CBPP_CurrentModuleInfo.WindowSize.x;
	int H = (int)CBPP_CurrentModuleInfo.WindowSize.y;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	CBPP_MainWindow = glfwCreateWindow(W, H, CBPP_CurrentModuleInfo.WindowTitle, NULL, NULL);
	
	if(CBPP_MainWindow == NULL){
		cbpp::DisplayError("ERROR", "Failed to create GLFW window!", true);
	}
	
	glfwMakeContextCurrent(CBPP_MainWindow);
	
	glfwSetFramebufferSizeCallback(CBPP_MainWindow, ReshapeHook);  
	
	glfwSwapInterval(1);
}

void RenderFrame() {
	cbdraw::SetColor(Color(255,255,0,255));
	cbdraw::TriangleOutline(Vec2(-15,0), Vec2(0,15), Vec2(15,0), 2.0f);
}

void MainLoop(){
	while(!glfwWindowShouldClose(CBPP_MainWindow) && CBPP_MainLoopCheck()){		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//uint32_t current_gs_id = cbpp::GetCurrentGameState();
		
		//if(current_gs_id != CB_STATEINDEX_MAX){
			//cbpp::GameState* current_gs = cbpp::GetStateRegList()[current_gs_id];
			//if(current_gs != nullptr){
				//current_gs->Tick();
			//}
		//}
		
		RenderFrame();
		
		glfwSwapBuffers(CBPP_MainWindow);
		glfwPollEvents();
	}
}

void ClearLogs() {
	FILE* errlog = fopen("logs/error.txt", "wt");
	fclose(errlog);
	
	FILE* gl_load_log_f = fopen("logs/gl_load.txt", "wt");
	fclose(gl_load_log_f);
}

void Cleanup(){
	cbpp::CleanupGameStates();
	
	glfwTerminate();
}

int main(int argc, char** argv){	
	SetUnhandledExceptionFilter(CBPP_ExceptHandle);
	
	ClearLogs();
	
	if(!glfwInit()){
		cbpp::DisplayError("GLFW Error", "Failed to init GLFW");
	}
	
	glfwSetErrorCallback(ErrorHook);
	
	ParseGameFile();
	LoadGameLibrary();
	
	CBPP_CreateWindow();
	bool gl_load_status = cbpp::LoadGL();
	
	glEnable(GL_MULTISAMPLE);
	
	if(!gl_load_status){
		cbpp::DisplayError("OpenGL error", "Failed to load OpenGL 4.0");
	}
	
	if(!CBPP_ModuleMain(argc, argv)){
		return 1;
	}
	
	cbent::Entity test;
	test.CreateComponent<cbent::TestComponent>();
	test.GetComponent<cbent::TestComponent>()->Print();
	
	int W = (int)CBPP_CurrentModuleInfo.WindowSize.x;
	int H = (int)CBPP_CurrentModuleInfo.WindowSize.y;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glViewport(0,0,W,H);
	cbdraw::SetWH(W, H);
	cbdraw::SetScale(0.025f);
	
	cbdraw::Init();
	
	CbThrowError("this is error system test");
	
	MainLoop();
	
	Cleanup();
	
	return 0;
}