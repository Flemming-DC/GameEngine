#include "ErrorChecker.h"
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32 
    #include <windows.h>
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

    void SetDebugFlag() 
    { 
        std::cout << "SetDebugFlag" << std::endl;
        debugFlag = true; 
    }
    bool DebugFlag() { return debugFlag; }

    inline bool IsBadReadPtr(void* ptr)
    {
        std::cout << "IsBadReadPtr 0" << std::endl;
        #ifdef _WIN32 
        MEMORY_BASIC_INFORMATION mbi = { 0 };
        if (::VirtualQuery(ptr, &mbi, sizeof(mbi)))
        {
            if (mbi.BaseAddress == nullptr)
                return false; // dont catch nullptr. Catch invalid pointer.
            DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
            bool isBad = !(mbi.Protect & mask);
            if (isBad)
                std::cout << "IsBadReadPtr 1" << std::endl;
            // check the page is not a guard page
            if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) 
                isBad = true;
            if (isBad)
                std::cout << "IsBadReadPtr 2" << std::endl;
            return isBad;
        }
        std::cout << "IsBadReadPtr 3" << std::endl;
        return true;
        //ErrorChecker::_RaiseError("Invalid pointer", func, file, line);

        #endif
        return false;
    }

    void _CheckPtr(void* ptr, const char* func, const char* file, int line)
    {
        if (IsBadReadPtr(ptr))
            ErrorChecker::_RaiseError("Invalid pointer", func, file, line);
    }
}
