#include "cb_main/glfw_callbacks.h"

void ReshapeHook(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void KeyPressHook(GLFWwindow* window, int key, int scancode, int action, int mods){
	
}