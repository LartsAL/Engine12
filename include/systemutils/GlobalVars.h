#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using WindowID = GLuint;
using ObjectID = GLuint;
using SceneID = GLuint;
using ComponentID = GLuint;

extern GLuint g_screenWidth;
extern GLuint g_screenHeight;

extern GLfloat g_deltaTime;

//extern std::shared_ptr<GLFWwindow> g_currentWindow;

extern WindowID g_currentWindow;

//extern void (*g_keyCallback)            (GLFWwindow*, int, int, int, int);
//extern void (*g_cursorPositionCallback) (GLFWwindow*, double, double);
//extern void (*g_cursorEnterCallback)    (GLFWwindow*, int);
//extern void (*g_mouseButtonCallback)    (GLFWwindow*, int, int, int);
//extern void (*g_scrollCallback)         (GLFWwindow*, double, double);
