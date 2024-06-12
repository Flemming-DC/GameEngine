#include "ErrorChecker.h"
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32 
    #pragma warning(push)
    #pragma warning(disable: 5105)
    #include <windows.h>
    #pragma warning(pop)
    #include <dbghelp.h>
#endif


namespace ErrorChecker
{

    static bool debugFlag = false;


    /* -- linux and mac back trace (from chatGPT)
    #include <iostream>
    #include <cstdlib>
    void PrintBacktrace()
    {
        constexpr int MAX_STACK_FRAMES = 64;
        void* stackTraces[MAX_STACK_FRAMES];
        int numFrames = backtrace(stackTraces, MAX_STACK_FRAMES);
        char** symbols = backtrace_symbols(stackTraces, numFrames);
        if (symbols == nullptr)
        {
            std::cerr << "Failed to obtain backtrace symbols\n";
            return;
        }
        for (int i = 0; i < numFrames; ++i)
        {
            std::cout << symbols[i] << "\n";
        }
        free(symbols);
    }
    */

    // currently, the Backtrace is windows only
    void PrintBacktrace()
    {
        #ifdef _WIN32 
        if (!SymInitialize(GetCurrentProcess(), nullptr, TRUE))
        {
            DWORD dwError = GetLastError();
            std::cerr << "PrintBacktrace: Failed to initialize. ErrorCode: " << dwError << std::endl;
            return;
        }

        constexpr int MAX_STACK_FRAMES = 64;
        void* stackTraces[MAX_STACK_FRAMES];
        WORD numFrames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, stackTraces, nullptr);

        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        if (symbol == nullptr)
        {
            std::cerr << "PrintBacktrace: Failed to allocate memory\n" << std::endl;
            return;
        }
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        std::vector<std::string> functionNames;
        int offset = 1; // this will eliminate _PrintBacktrace from the backtrace
        for (WORD i = offset; i < numFrames; ++i)
        {
            bool success = SymFromAddr(GetCurrentProcess(), (DWORD64)(stackTraces[i]), 0, symbol);
            if (!success)
            {
                DWORD dwError = GetLastError();
                std::cerr << "PrintBacktrace: Failed to get function name from address. ErrorCode = "
                    << dwError << "\n" << std::endl;
                break;
            }
            if (std::string(symbol->Name) == "main")
                break;
            functionNames.push_back(std::string(symbol->Name));
        }

        for (int i = functionNames.size() - 1; i >= 0; i--)
            std::cerr << "    Function: " << functionNames[i] << std::endl;
        free(symbol);
        #endif
    }

    void _SetDebugFlag(const char* func, const char* file, int line)
    {
        std::cout << "SetDebugFlag in " << func << " at " << file << ":" << line << std::endl;
        debugFlag = true; 
    }
    bool _GetDebugFlag() { return debugFlag; }

    void _RaiseError(std::string message, const char* func, const char* file, int line)
    {
        std::cout << "\nBackTrace: " << std::endl;
        PrintBacktrace();
        std::cout << "----------" << std::endl;
        std::cout << "ERROR MESSAGE: " << message << "\nERROR in " << func << " at " << file << ":" << line << std::endl;
        std::cout << "----------\n" << std::endl;
        std::exit(1);
    }

    void _Warning(std::string message, const char* func, const char* file, int line)
    {
        std::cout << "WARNING MESSAGE: " << message << "\nWARNING in " << func << " at " << file << ":" << line << std::endl;
    }
}
