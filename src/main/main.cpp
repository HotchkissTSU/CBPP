#include "cbpp.h"

#include "cbpp/yyjson.h"

#include "cbvs/error_check.h"

#include <stdio.h>
#include <typeinfo>
#include <cstring>
#include <signal.h>
#include <locale.h>

#include "GLFW/glfw3.h"

#include "cb_main/gamedata.h"
#include "cb_main/aux.h"

#ifdef __linux__
	#include <dlfcn.h>
#endif

#define CBEP_FUNCHK(fname) if(ModuleData.fname == NULL) { CbThrowErrorf( "Game module has no '%s' function", #fname ); return false; }
#define CBEP_LOGC(lname) { char path[128]; snprintf(path, 128, "logs/%s.txt", #lname); FILE* io_ptr = fopen( path, "wt" ); fclose(io_ptr); }

using namespace cbpp;

static sighandler_t _old_sig_handler;

static double g_fPrevMX = 0.0, g_fPrevMY = 0.0;

void error_callback(int error_code, const char* description){
	char err_log[256];
	snprintf(err_log, 256, "GLFW error: [%i] -> %s\n", error_code, description);
	CbThrowError(err_log);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	double f_dx = xpos - g_fPrevMX;
	double f_dy = ypos - g_fPrevMY;

	static cbpp::Event ev;
	CreateEvent(&ev);

	ev.Type = Event::MOUSE_MOVE;

	ev.Data.MouseMove.dX = (float_t)f_dx;
	ev.Data.MouseMove.dY = (float_t)f_dy;
	ev.Data.MouseMove.X = (uint32_t)xpos;
	ev.Data.MouseMove.Y = (uint32_t)ypos;

	cbpp::PushEvent(&ev);

	g_fPrevMX = xpos; g_fPrevMY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static cbpp::Event ev;
	CreateEvent(&ev);

	ev.Type = Event::KEYBOARD;

	ev.Data.Keyboard.Key = (uint32_t)key;
	ev.Data.Keyboard.Mods = (uint32_t)mods;
	ev.Data.Keyboard.Scancode = (uint32_t)scancode;

	cbpp::PushEvent(&ev);
}

void text_input_callback(GLFWwindow* window, unsigned int codepoint) {
	/*cbpp::Event ev;

	ev.Type = CBPP_TEXTINPUT_EVENT;

	ev.TextInputEvent.Charcode = (char32_t)codepoint;*/
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);

	cbvs::g_vScreenSize.x = (float_t)width;
	cbvs::g_vScreenSize.y = (float_t)height;

	cbvs::g_fScreenRatio = (float_t)width / (float_t)height;
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
				GameData.WindowW = cbpp::math::Clamp( atoi(next), 100, 10000 );
			}
		}

		if( strcmp(comm, "-h") == 0 ) {
			if(next != NULL) {
				GameData.WindowH = cbpp::math::Clamp( atoi(next), 100, 10000 );
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
	printf("Loading game library: '%s'\n", GameData.GameLibrary);
	void* lib_handle = dlopen(lname, RTLD_NOW);
	
	if(lib_handle == NULL){
		//printf("dlopen error: %s\n", dlerror());
		char errlog[256];
		snprintf(errlog, 256, "dlopen error: %s", dlerror());
		CbThrowError(errlog);
		return false;
	}

	ModuleData.LibHandle = lib_handle;
	
	bool (*ModuleLoopCheck_)( void ) = ( bool (*)() )dlsym(lib_handle, "ModuleLoopCheck");
	if(ModuleLoopCheck_ != NULL) {
		ModuleData.ModuleLoopCheck = ModuleLoopCheck_;
	}
	
	ModuleData.ModuleMain = ( bool (*)(int, char**) )dlsym(lib_handle, "ModuleMain");
	ModuleData.ModuleTick = ( void (*)() )dlsym(lib_handle, "ModuleTick");
	ModuleData.ModulePreMain = ( void (*)() )dlsym(lib_handle, "ModulePreMain");
	ModuleData.ModuleEventCallback = ( bool (*)(cbpp::Event&) )dlsym(lib_handle, "ModuleEventCallback");

	CBEP_FUNCHK(ModuleMain);
	CBEP_FUNCHK(ModuleTick);
	CBEP_FUNCHK(ModulePreMain);
	CBEP_FUNCHK(ModuleEventCallback);
	
	return true;
}
#endif

#ifdef _WIN64
bool LoadGameLibrary() {
	static_assert(true, "Windows is not fully supported yet");
	return false;
}
#endif

void __mount_group(yyjson_val* jValue, const char* sName, cbpp::SEARCH_PATH iIndex) noexcept {
	yyjson_val* jGValue = yyjson_obj_get(jValue, sName);
	if(jGValue == NULL) { return; }

	if(yyjson_is_str(jGValue)) { return; }
	
	size_t iListLength = yyjson_get_len(jGValue);
	for(size_t i = 0; i < iListLength; i++) {
		yyjson_val* jCurrent = yyjson_arr_get(jGValue, i);
		if(!yyjson_is_str(jCurrent)) { continue; }
		cbpp::MountSearchPath(iIndex, yyjson_get_str(jCurrent));
	}
}

void MountSearchPaths(yyjson_val* jValue) noexcept {
	__mount_group(jValue, "all", 	 cbpp::PATH_ALL);
	__mount_group(jValue, "map", 	 cbpp::PATH_MAP);
	__mount_group(jValue, "locale",  cbpp::PATH_LOCALE);
	__mount_group(jValue, "texture", cbpp::PATH_TEXTURE);
	__mount_group(jValue, "sound",   cbpp::PATH_SOUND);
	__mount_group(jValue, "shader",  cbpp::PATH_SHADER);
    __mount_group(jValue, "font",    cbpp::PATH_FONT);
}

bool LoadGamefile() {
	char* gf = GameData.Gamefile;
	char* glib = NULL;

	cbpp::File hInput(gf, "rt");
	size_t iFileLen = hInput.Length();

	char* sJsonBuffer = (char*)malloc(iFileLen+1);
	sJsonBuffer[iFileLen] = '\0';

	hInput.Read( sJsonBuffer, iFileLen );

	yyjson_doc* jDoc = yyjson_read_opts(sJsonBuffer, iFileLen, CBPP_JSONREAD_OPTS, NULL, NULL);

	if(jDoc == NULL) {
		CbThrowError("Failed to parse gamefile JSON");
	}

	yyjson_val* jRoot = yyjson_doc_get_root(jDoc);

	free(sJsonBuffer);

	if(jRoot == NULL) {
		CbThrowError("Json root node is NULL");
		return false;
    }
    
	yyjson_val* jGamelib = yyjson_obj_get(jRoot, "gamelib");

	if(jGamelib != NULL && yyjson_is_str(jGamelib)) {
		const char* sTmp = yyjson_get_str(jGamelib);
		glib = strdup(sTmp);
    }

    yyjson_val* jRess = yyjson_obj_get(jRoot, "resolutions");
    if(jRess != NULL) {
        RegisterScreenResolutions(jRess);
    }else{
        CbThrowWarning("No screen resolutions are providen");
    }

	yyjson_val* jSearchPaths = yyjson_obj_get(jRoot, "paths");
	if(jSearchPaths != NULL) {
		MountSearchPaths(jSearchPaths);
	}else{
		CbThrowWarning("No search paths are providen. This is probably kinda bad");
    }

	yyjson_val* jShaders = yyjson_obj_get(jRoot, "shaders");
	if(jShaders != NULL) {
		if(!RegisterShaders(jShaders)) {
			CbThrowError("Shader registration has failed. See log for details");
		}
	}

	yyjson_val* jLocales = yyjson_obj_get(jRoot, "locales");
	if(jLocales != NULL) {
		yyjson_val *jValue, *jKey;
		size_t i, iMax;

		yyjson_obj_foreach(jLocales, i, iMax, jKey, jValue) {
			const char* sName = jKey->uni.str;
			const char* sFile = yyjson_get_str(jValue);

			MountLocale(sName, sFile);
		}
	}

	yyjson_doc_free(jDoc);

	if(glib == NULL) {
		CbThrowError("Failed to get 'gamelib' value from the gamefile");
		return false;
	}else{
		GameData.GameLibrary = glib;
	}

	return true;
}

void PrintSearchPaths(FILE* hStream = stdout) {
	fprintf(hStream, "Engine default search paths:\n");
	for(size_t i = 0; i < cbpp::SPATHS_AMOUNT; i++) {
		fprintf(hStream, "\tGroup %s:\n", cbpp::SearchPathGroupName((cbpp::SEARCH_PATH)i));
		for(size_t j = 0; j < cbpp::g_aSearchPaths[i].Length(); j++) {
			fprintf(hStream, "\t\t%s\n", (const char*)cbpp::g_aSearchPaths[i].At(j));
		}
	}
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

void ClearLogs() {
	CBEP_LOGC(error);
}

void Cleanup() {
	free(GameData.WindowTitle);
	free(GameData.Gamefile);
	free(GameData.GameLibrary);

    cbvs::CleanupRender();
    CleanupSprites();

	//ddraw::Cleanup();
	//cbvs::CleanupDefaultShaders();

	glfwTerminate();

	if(ModuleData.LibHandle != NULL) {
		dlclose(ModuleData.LibHandle);
    }

	free(g_string_buffer);
}

void SetSignalsCallback(sighandler_t cb_sig_handler) {
	signal(SIGSEGV, cb_sig_handler);
	signal(SIGTERM, cb_sig_handler);
	//signal(SIGABRT, cb_sig_handler); never uncomment it, or our poor OS will fall into an infinite loop
	signal(SIGFPE, cb_sig_handler);
	signal(SIGILL, cb_sig_handler);
	signal(SIGINT, cb_sig_handler);
}

void cb_sig_handle(int sigid) {
	CbThrowErrorf("%s (%d)", strsignal(sigid), sigid);
	exit(-1);
}

int main( int argc, char** argv ) {
	SetSignalsCallback(cb_sig_handle);

	setlocale(LC_ALL, "");
	ClearLogs();

	ResolveArgs( argc, argv );
	glfwSetErrorCallback(error_callback);

	if( !LoadGamefile() ) {
		CbThrowErrorf("Failed to parse gamefile '%s'", GameData.Gamefile);
		exit(-1);
	}

	if( !LoadGameLibrary() ) {
		CbThrowErrorf("Failed to load game library '%s'", GameData.GameLibrary);
		exit(-1);
	}

	if( !glfwInit() ) {
		CbThrowError("Failed to initialize GLFW library");
		exit(-1);
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

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
	
	ModuleData.ModulePreMain();
	if(!cbvs::LoadShaders()) {
		CbThrowWarning("Shaders loading has failed, so everything will fall apart very likely");
	}

	if(!ddraw::Init()) {
		CbThrowError("Failed to init DDRAW");
	}

    if(!cbvs::InitRender()) {
        CbThrowError("Failed to init main rendering");
    }

	glfwSetCursorPosCallback(GameData.MainWindow, cursor_position_callback);
	glfwSetWindowSizeCallback(GameData.MainWindow, window_size_callback);
	glfwSetKeyCallback(GameData.MainWindow, key_callback);

	/*
	cbvs::ScreenSize.x = (float_t)GameData.WindowW;
	cbvs::ScreenSize.y = (float_t)GameData.WindowH;
	cbvs::ScreenRatio = cbvs::ScreenSize.x / cbvs::ScreenSize.y;*/
	glViewport(0,0,GameData.WindowW, GameData.WindowH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	PrintGLInfo();
	PrintSearchPaths();

	/*if(!cbvs::InitDefaultShaders()) {
		exit(-1);
	}

	bool ddraw_init_res = ddraw::Init();
	if(!ddraw_init_res) {
		CbThrowError( "Failed to init DDRAW" );
		exit(-1);
	}*/

    LoadSheet("cbpp_base");

	if(!ModuleData.ModuleMain( argc, argv )) {
		CbThrowError("Unable to proceed, ModuleMain has returned FALSE");
		exit(-1);
    }
    
	//ddraw::SetColor( cbpp::Color(255,0,0,255) );
	
	cbpp::Event ev;
	
	while( !glfwWindowShouldClose(GameData.MainWindow) && ModuleData.ModuleLoopCheck() ) {
		glfwPollEvents();

		while ( cbpp::PollEvent(&ev) ) {
			if(ModuleData.ModuleEventCallback(ev)) { continue; }

			switch (ev.Type) {
				
			}
        }

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ModuleData.ModuleTick();

        cbvs::RenderSpriteFinish();
		
		glfwSwapBuffers(GameData.MainWindow);
	}
	
	Cleanup();

    return 0;
}
