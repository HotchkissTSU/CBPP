#include "cb_main/glfw_callbacks.h"
#include <stdio.h>

void ReshapeHook(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void KeyPressHook(GLFWwindow* window, int key, int scancode, int action, int mods){
	
}

void error_callback(int error, const char* description) {
	printf("Error: %s\n", description);
}