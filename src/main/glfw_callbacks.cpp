#include "cb_main/glfw_callbacks.h"

#include "cbvs/draw.h"
#include <stdio.h>

void ReshapeHook(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
	cbdraw::SetWH(width, height);
}

void KeyPressHook(GLFWwindow* window, int key, int scancode, int action, int mods){
	
}

void ErrorHook(int error, const char* description) {
	printf("GLFW error: %s\n", description);
}