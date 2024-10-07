#include "cbpp.h"

#include "json/json.h"

#include "cb_main/interface.h"
#include "cb_main/settings.h"

#include "cbvs/error_check.h"

#include <stdio.h>
#include <typeinfo>

#include "GLFW/glfw3.h"

#ifdef __linux__
#include <dlfcn.h>
#endif

using namespace cbpp;
using namespace cbent;

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
	printf("LNAME = %s\n", lname);
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

static const float test_buffer[] = {
	-0.5f, 0.5f,
	0.5f, 0.5f,
	0.0f, -0.5f
};

int main(int argc, char** argv) {
	FILE* errlog_ptr = fopen("logs/error.txt", "w");
	fclose(errlog_ptr);

	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		CbThrowError("Failed to initialize GLFW library");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* wnd = glfwCreateWindow(600, 600, "test #2", NULL, NULL);
	if(wnd == NULL) { CbThrowError("Window is not created"); }

	glfwMakeContextCurrent(wnd);

	int version = gladLoadGL();
	if(version == 0) {
		CbThrowError("Failed to load OpenGL 4.0");
		return -1;
	}

	PrintGLInfo();

	glViewport(0,0,600,600);

	GLuint vao, vbo;

	cbpp::File inp("assets/sequences/test.cbseq", "rt");
	char* buff = new char[ inp.Length() + 1 ];
	memset(buff, 0, inp.Length()+1);
	inp.Read(buff, inp.Length());

	//printf("%s\n", buff);

	std::string sc_src(buff);
	delete[] buff;

	cbpp::SequenceScript sc_test;
	sc_test.Interprete( sc_src, true );

	cbvs::Shader* ddraw_vtx_shader = new cbvs::Shader(GL_VERTEX_SHADER, "assets/shaders/default.vertex");
    cbvs::Shader* ddraw_frag_shader = new cbvs::Shader(GL_FRAGMENT_SHADER, "assets/shaders/default.fragment");
    cbvs::ShaderProgram* ddraw_def_prog = new cbvs::ShaderProgram(ddraw_vtx_shader, ddraw_frag_shader, NULL);
	ddraw_def_prog->LinkProgram();

	cbvs::RegisterProgram(ddraw_def_prog, "ddraw_default");

	ddraw_def_prog->PushUniform("ddraw_COLOR", 1.0f, 0.0f, 0.0f, 1.0f);

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(test_buffer), test_buffer, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	sc_test.Unlock();

	printf("%s\n", sc_test.GetErrorLog().c_str());

	sc_test.PrintDebug();

	while( !glfwWindowShouldClose(wnd) ) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ddraw_def_prog->Use();

		glBindVertexArray(vao);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(test_buffer), test_buffer);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		sc_test.Update();

		glfwPollEvents();
		glfwSwapBuffers(wnd);
	}

	return 0;
}

int main2(int argc, char** argv) {	
	ResolveArgs(argc, argv);

	FILE* errlog_ptr = fopen("logs/error.txt", "w");
	fclose(errlog_ptr);

	if( !LoadGameLibrary("./test.so") ) {
		//say something about this
		CbThrowError("Failed to load game module!");
		exit(1);
	}
	
	int W = 600;
	int H = 600;
	
	bool api_load = glfwInit();

	if(!api_load) {
		CbThrowError("Failed to initialize GLFW");
		exit(1);
	}

	glfwSetErrorCallback(error_callback);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* CBPP_MainWindow = glfwCreateWindow(W, H, "test", NULL, NULL);

	if(!CBPP_MainWindow) {
		CbThrowError("Failed to create GLFW window");
	}
	
	glfwMakeContextCurrent(CBPP_MainWindow);

	int version = gladLoadGL();
	if(version == 0) {
		CbThrowError("Failed to load OpenGL 4.0");
		return -1;
	}

	PrintGLInfo(stdout);
	
	glfwSwapInterval(1);
	
	glViewport(0,0,W,H);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if( !ModuleData.ModuleMain(argc, argv) ) {
		exit(1);
	}

	if(ddraw::Init() != GL_NO_ERROR) {
		printf("Failed to init DDRAW\n");
	}

	while( !glfwWindowShouldClose(CBPP_MainWindow) || !ModuleData.ModuleLoopCheck() ){		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//cbdraw::CircleOutline(Vec2(0), 100.0f);
		//ModuleData.ModuleTick();

		//ddraw::Line(Vec2(0), Vec2(1));

		//test.Update();
		
		glfwSwapBuffers(CBPP_MainWindow);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}
