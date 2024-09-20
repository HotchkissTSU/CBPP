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

	//std::string sanitized;
	//CBSEQ_Sanitize(ftext, sanitized);
	//printf("%s\n", sanitized.c_str());

	File inp("assets/sequences/test.cbseq", "rt");
	char* buff = new char[inp.Length()+1];
	memset(buff, 0, inp.Length()+1);

	inp.Read((uint8_t*)buff, inp.Length(), 1);

	std::string ftext(buff);

	printf("FILE TEXT: %s\n", ftext.c_str());

	delete[] buff;
	
	SequenceScript test;
	test.Interprete(ftext, true);

	cbseq_words_t blocks;
	test.GetBlocksNames(blocks);

	for(int i = 0; i < blocks.size(); i++) {
		printf("BLOCK : %s\n", blocks[i]);
		std::string blk_name = blocks[i];
		CBSEQ_Block_t blk = test.GetBlockCode(blk_name);
		for(int j = 0; j < blk.prog.size(); j++) {
			CBSEQ_ccom_t ccom = blk.prog[j];
			printf("\tComID: %d | ", ccom.comId);

			for(int k = 0; k < ccom.args.size(); k++) {
				CBSEQ_arg_t arg = ccom.args[k];
				if(arg.argType == CBSEQ_VTYPE_NUMBER) {
					printf("%f ", arg.numValue);
				}else{
					printf("'%s' ", arg.strValue.c_str());
				}
			}

			printf("\n");
		}
	}
	
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
