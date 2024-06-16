#pragma once
#include <iostream>
#include "logger.h"
#include "Profiler.h" // this line saves me from having to include profiler over and over again

#define RaiseError(...) ErrorChecker::_RaiseError(logger::make_string(__VA_ARGS__), __func__, __FILE__, __LINE__) 
#define Warning(...) ErrorChecker::_Warning(logger::make_string(__VA_ARGS__), __func__, __FILE__, __LINE__)  

#ifdef _DEBUG
#define SetDebugging ErrorChecker::_SetDebugFlag(__func__, __FILE__, __LINE__);
#define RunIfDebugging(codeLine) if (ErrorChecker::_GetDebugFlag()) codeLine;
#define IfDebugging if (ErrorChecker::_GetDebugFlag())
#endif // _DEBUG


#ifdef _DEBUG
#define InDebug(codeLine) codeLine
#else
#define InDebug(codeLine) // evaluate to nothing
#endif

#define Deny(condition, ...)   InDebug( if (  condition ) RaiseError(logger::make_string(__VA_ARGS__)) )
#define Assert(condition, ...) InDebug( if (!(condition)) RaiseError(logger::make_string(__VA_ARGS__)) )

namespace ErrorChecker
{
    void PrintBacktrace();
    void _SetDebugFlag(const char* func, const char* file, int line);
    bool _GetDebugFlag();
    void _Warning(std::string message, const char* func, const char* file, int line);
    [[noreturn]] void _RaiseError(std::string message, const char* func, const char* file, int line);


}

