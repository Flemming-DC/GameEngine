#pragma once
#include <iostream>
#include "logger.h"

#define RaiseError(...) ErrorChecker::_RaiseError(logger::make_string(__VA_ARGS__), __func__, __FILE__, __LINE__) 
#define Warning(...) ErrorChecker::_Warning(logger::make_string(__VA_ARGS__), __func__, __FILE__, __LINE__)  

#ifdef _DEBUG
#define SetDebug ErrorChecker::_SetDebugFlag(__func__, __FILE__, __LINE__);
#define IfDebug(codeLine) if (ErrorChecker::_GetDebugFlag()) codeLine;
#endif // _DEBUG

namespace ErrorChecker
{
    void PrintBacktrace();
    void _SetDebugFlag(const char* func, const char* file, int line);
    bool _GetDebugFlag();
    void _Warning(std::string message, const char* func, const char* file, int line);
    [[noreturn]] void _RaiseError(std::string message, const char* func, const char* file, int line);


}

