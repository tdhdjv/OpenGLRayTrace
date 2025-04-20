#include "Core.h"

#include <iostream>
#include <unordered_map>

static float lastTime;
static std::unordered_map<const char*, int> expressionCalledCount;
static std::unordered_map<const char*, float> expressionElapsedTime;

void measureStartTime() {
    lastTime = glfwGetTime(); 
}

void printTime(const char* expression, unsigned count) {
    float currentTime = glfwGetTime();
    float elaspedTime = currentTime - lastTime;

    if (count <= 1) {
        float estimate = round(100.0 * 1000.0 * elaspedTime) / 100.0;
        std::cout << "Time Taken for executing [" << expression << "] was " << estimate << "ms" << std::endl;
        return;
    }

    if (!expressionCalledCount.contains(expression)) {
        expressionCalledCount[expression] = 0;
        expressionElapsedTime[expression] = 0.0;
    }

    expressionCalledCount[expression]++;
    expressionElapsedTime[expression] += elaspedTime;
    
    if (expressionCalledCount[expression] >= count) {
        float time = expressionElapsedTime[expression];
        float estimate = round(100.0 * 1000.0 * time) / 100;
        float estimate_avg = round(100.0 * 1000.0 * (time / float(count))) / 100;

        expressionCalledCount[expression] = 0;
        expressionElapsedTime[expression] = 0.0;
        std::cout << "Time Taken for executing [" << expression << "] (" << count <<" Times) was on [Average: " << estimate_avg << "ms" << ", Total: " << estimate << "ms]" << std::endl;
    }
}

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

const bool GLCheckError(const char* file, unsigned int line) {
    unsigned int error;
    bool isError = true;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "[OpenGL Error: " << GLParse(error) << "] at " << file << " line: " << line << std::endl;
       
        isError = false;
    }
    return isError;
}

const char* GLParse(unsigned int error) {
    switch (error) {
    case GL_INVALID_ENUM:
        return "Invalid enum";
    case GL_INVALID_VALUE:
        return "Invalid value";
    case GL_INVALID_OPERATION:
        return "Invalid operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation";
    case GL_OUT_OF_MEMORY:
        return "Out of memory";
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
    default:
        return "Unknow Error";
    }
}