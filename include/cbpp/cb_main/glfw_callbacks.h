#ifndef CBPP_EP_GLFW_CALLBACKS_H
#define CBPP_EP_GLFW_CALLBACKS_H

#include "glad/gl.h"
#include "GLFW/glfw3.h"

void ReshapeHook(GLFWwindow* window, int width, int height);
void KeyPressHook(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif