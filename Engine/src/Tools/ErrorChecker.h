#pragma once
#include <GL/glew.h>
#include <iostream>

//#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) _glClearError();\
    x;\
    _glCall(#x, __FILE__, __LINE__)
#define RaiseError(message) _RaiseError(message, __func__, __FILE__, __LINE__)  // using namespace std::string_literals;
#define Warning(message) _Warning(message, __func__, __FILE__, __LINE__)  // using namespace std::string_literals;

template<typename T> [[noreturn]] 
void _RaiseError(T message, const char* func, const char* file, int line)
{
    std::cout << "\nBackTrace: " << std::endl;
    PrintBacktrace();
    std::cout << "----------" << std::endl;
    std::cout << "ERROR MESSAGE: " << message << "\nERROR in " << func << " at " << file << ":" << line << std::endl;
    std::cout << "----------\n" << std::endl;
    std::exit(1);
}

template<typename T>
void _Warning(T message, const char* func, const char* file, int line)
{
    std::cout << "WARNING MESSAGE: " << message << "\nWARNING in " << func << " at " << file << ":" << line << std::endl;
}

template<typename T>
void Log(T message)
{
    std::cout << message << std::endl;
}


void _glClearError();
void _glCall(const char* function, const char* file, int line);

bool _LastGlCallHadError();

void PrintBacktrace();

void SetDebugFlag();
bool DebugFlag();

