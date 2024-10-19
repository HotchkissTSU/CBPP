#include "cbpp.h"

#include "json/libjson.h"

#include "cbvs/error_check.h"

#include <stdio.h>
#include <typeinfo>
#include <cstring>

#include "GLFW/glfw3.h"

#include "cb_main/gamedata.h"

#ifdef __linux__
#include <dlfcn.h>
#endif

using namespace cbpp;
using namespace cbent;

void error_callback(int error_code, const char* description){
	char err_log[256];
	snprintf(err_log, 256, "GLFW error: [%i] -> %s\n", error_code, description);
	CbThrowError(err_log);
}

void ResolveArgs(int argc, char** argv) {
	for(uint16_t i = 0; i < argc; i++) {
		char* comm = argv[i];
		char* next = NULL;

		if( i < argc+1 ) {
			next = argv[i+1];
		}

		if( strcmp(comm, "-game") == 0 ) {
			if(next != NULL) {
				GameData.Gamefile = strdup(next);
			}
		}
		
		if( strcmp(comm, "-w") == 0 ) {
			if(next != NULL) {
				GameData.WindowW = cbpp::Clamp( atoi(next), 100, 10000 );
			}
		}

		if( strcmp(comm, "-h") == 0 ) {
			if(next != NULL) {
				GameData.WindowH = cbpp::Clamp( atoi(next) , 100, 10000 );
			}
		}

		if( strcmp(comm, "-title") == 0 ) {
			if(next != NULL) {
				GameData.WindowTitle = strdup(next);
			}
		}
	}

	if(GameData.Gamefile == NULL) {
		GameData.Gamefile = strdup( GameData.DefaultGamefile );
	}
}

#ifdef __linux__
bool LoadGameLibrary() {	
	char* lname = GameData.GameLibrary;
	void* lib_handle = dlopen(lname, RTLD_NOW);
	
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

#ifdef _WIN64
	bool LoadGameLibrary() {
		static_assert(true, "Windows is not fully supported yet");
		return false;
	}
#endif

bool LoadGamefile() {
	char* gf = GameData.Gamefile;
	char* glib = NULL;

	cbpp::File json_input(gf, "rt");

	char* buffer = new char[ json_input.Length() + 1 ];
	memset(buffer, 0, json_input.Length() + 1);

	json_input.Read( buffer, json_input.Length() );

	JSONNODE* root = json_parse( buffer );

	delete[] buffer;

	if(root == NULL) {
		CbThrowError("Json root node is NULL");
		return false;
	}

	JSONNODE* j_gamelib = json_get(root, JSON_TEXT("gamelib"));

	if(j_gamelib) {
		json_char* j_text = json_as_string(j_gamelib);
		glib = strdup((char*)j_text);
		json_free(j_text);
	}

	json_delete(root);

	if(glib == NULL) {
		CbThrowError("Failed to get 'gamelib' value from the gamefile");
		return false;
	}else{
		GameData.GameLibrary = glib;
	}

	return true;
}

void PrintGLInfo(FILE* stream = stdout, uint8_t debug_table_width = 55) {
	uint8_t dtw_left = 11, dtw_right = debug_table_width - dtw_left - 3;

	fprintf(stream, "┌");
	for(int i = 0; i < debug_table_width; i++) { fprintf(stream, "─"); }
	fprintf(stream, "┐\n");
	fprintf(stream, "│ %-*.*s│%*.*s │\n", dtw_left, dtw_left, "OpenGL ver", dtw_right, dtw_right, glGetString(GL_VERSION));
	fprintf(stream, "│ %-*.*s│%*.*s │\n", dtw_left, dtw_left, "GLSL ver", dtw_right, dtw_right, glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(stream, "│ %-*.*s│%*.*s │\n", dtw_left, dtw_left, "GPU vendor", dtw_right, dtw_right, glGetString(GL_VENDOR));
	fprintf(stream, "│ %-*.*s│%*.*s │\n", dtw_left, dtw_left, "Renderer", dtw_right, dtw_right, glGetString(GL_RENDERER));
	fprintf(stream, "└");
	for(int i = 0; i < debug_table_width; i++) { fprintf(stream, "─"); }
	fprintf(stream, "┘\n");
}

void Cleanup() {
	glfwTerminate();

	free(GameData.WindowTitle);
	free(GameData.Gamefile);
	free(GameData.GameLibrary);
}

int main( int argc, char** argv ) {
	ResolveArgs( argc, argv );
	glfwSetErrorCallback(error_callback);

	if( !LoadGamefile() ) {
		CbThrowErrorf("Failed to parse gamefile '%s'", GameData.Gamefile);
	}

	if( !LoadGameLibrary() ) {
		CbThrowErrorf("Failed to load game library '%s'", GameData.GameLibrary);
	}

	if( !glfwInit() ) {
		CbThrowError("Failed to initialize GLFW library");
		exit(-1);
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	ModuleData.ModuleWindowHint();

	if(GameData.WindowTitle == NULL) {
		GameData.WindowTitle = strdup( GameData.DefaultWindowTitle );
	}

	GameData.MainWindow = glfwCreateWindow( GameData.WindowW, GameData.WindowH, GameData.WindowTitle, NULL, NULL );

	if(GameData.MainWindow == NULL) {
		CbThrowError("Failed to create GLFW window");
		exit(-1);
	}

	glfwMakeContextCurrent( GameData.MainWindow );

	int load_ver = gladLoadGL();

	if(load_ver == 0) {
		CbThrowError("Failed to load OpenGL 4.0");
		exit(-1);
	}

	PrintGLInfo();

	bool ddraw_init_res = ddraw::Init();
	if(!ddraw_init_res) {
		CbThrowError( "Failed to init DDRAW" );
		exit(-1);
	}

	if(!ModuleData.ModuleMain( argc, argv )) {
		CbThrowError("ModuleMain returned FALSE");
		exit(-1);
	}
	
	ddraw::SetColor( cbpp::Color(255,0,0,255) );

	while( !glfwWindowShouldClose(GameData.MainWindow) && ModuleData.ModuleLoopCheck() ) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ModuleData.ModuleTick();
		
		glfwPollEvents();
		glfwSwapBuffers(GameData.MainWindow);
	}

	Cleanup();

	return 0;
}
