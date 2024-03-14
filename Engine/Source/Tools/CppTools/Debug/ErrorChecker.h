#pragma once
#include <iostream>

#define RaiseError(message) ErrorChecker::_RaiseError(message, __func__, __FILE__, __LINE__)  // using namespace std::string_literals;
#define Warning(message) ErrorChecker::_Warning(message, __func__, __FILE__, __LINE__)  // using namespace std::string_literals;

namespace ErrorChecker
{
    void PrintBacktrace();
    void SetDebugFlag();
    bool DebugFlag();

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



}

