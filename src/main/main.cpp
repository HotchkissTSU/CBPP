#include "cbpp.h"

#include "json/json.h"

#include "cb_main/interface.h"
#include "cb_main/settings.h"
#include "cb_main/glfw_callbacks.h"

#include <stdio.h>
#include <typeinfo>

#ifdef __linux__
#include <dlfcn.h>
#endif

using namespace cbpp;

bool ModuleLoopCheck_default() {
	return true;
}

struct ModuleData_t {
	bool (*ModuleLoopCheck)( void ) = ModuleLoopCheck_default;
	bool (*ModuleMain)( int, char** ) = nullptr;
	void (*ModuleTick)( void ) = nullptr;
	void (*ModuleWindowHint)( void ) = nullptr;
	
} ModuleData;

void error_callback(int error_code, const char* description){
	char err_log[256];
	snprintf(err_log, 256, "GLFW error: [%i] -> %s\n", error_code, description);
	CbThrowError(err_log);
}

void ResolveArgs(int argc, char** argv) {
	
}

#ifdef __linux__
bool LoadGameLibrary(const char* lname) {	
	void* lib_handle = dlopen(lname, RTLD_NOW | RTLD_LOCAL);
	
	if(lib_handle == NULL){
		//printf("dlopen error: %s\n", dlerror());
		char errlog[256];
		snprintf(errlog, 256, "dlopen error: %s", dlerror());
		CbThrowError(errlog);
		return false;
	}
	
	bool (*ModuleLoopCheck_)( void ) = ( bool (*)() )dlsym(lib_handle, "ModuleLoopCheck");
	if(ModuleLoopCheck_ != NULL) {
		ModuleData.ModuleLoopCheck = ModuleLoopCheck_;
	}
	
	ModuleData.ModuleMain = ( bool (*)(int, char**) )dlsym(lib_handle, "ModuleMain");
	ModuleData.ModuleTick = ( void (*)() )dlsym(lib_handle, "ModuleTick");
	ModuleData.ModuleWindowHint = ( void (*)() )dlsym(lib_handle, "ModuleWindowHint");
	
	if(ModuleData.ModuleMain == NULL || ModuleData.ModuleTick == NULL || ModuleData.ModuleWindowHint == NULL) {
		CbThrowError("Game module lacks neccesary functions");
		return false;
	}
	
	return true;
}
#endif

int main(int argc, char** argv) {
	glfwSetErrorCallback(error_callback);
	
	ResolveArgs(argc, argv);

	FILE* errlog_ptr = fopen("logs/error.txt", "w");
	fclose(errlog_ptr);
	
	if( !LoadGameLibrary("./test.so") ) {
		//say something about this
		CbThrowError("Failed to load game module!");
	}
	
	int W = 600;
	int H = 600;
	
	cbpp::LoadGL();
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* CBPP_MainWindow = glfwCreateWindow(W, H, "test", NULL, NULL);
	
	glfwMakeContextCurrent(CBPP_MainWindow);
	
	glfwSwapInterval(1);
	
	glViewport(0,0,W,H);
	
	if( !ModuleData.ModuleMain(argc, argv) ) {
		exit(1);
	}

	std::string test = "com1 \"arg1 quoted lmao\";\n/newline comment\ncom2 niggar; /after-line comment\n";
	std::string test_modded;

	CBSEQ_Sanitize(test, test_modded);

	printf("Default string:\n%s\n", test.c_str());
	printf("Modded string:\n%s\n", test_modded.c_str());

	while( !glfwWindowShouldClose(CBPP_MainWindow) || !ModuleData.ModuleLoopCheck() ){		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//cbdraw::CircleOutline(Vec2(0), 100.0f);
		ModuleData.ModuleTick();
		
		glfwSwapBuffers(CBPP_MainWindow);
		glfwPollEvents();
	}

	return 0;
}
