#pragma once

//frequently used includes
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

//Time Measureing Marcos
#define TIME(x) measureStartTime();\
x;\
printTime(#x, 1);\

#define TIME_C(x, count) measureStartTime();\
x;\
printTime(#x, count);\

#define TIME_L(x, label) measureStartTime();\
x;\
printTime(label, 1);\

#define TIME_CL(x, count, label) measureStartTime();\
x;\
printTime(label, count);\

void measureStartTime();
void printTime(const char* expression, unsigned count);

//Error Checking Marcos
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(__FILE__, __LINE__));\


void GLClearError();
const bool GLCheckError(const char* file, unsigned int line);
const char* GLParse(unsigned int error);