#include "ErrorChecker.h"
#include <iostream>
#include <string>
#include <vector>
#include <GL/glu.h>
#ifdef _WIN32 
    #include <windows.h>
    #include <dbghelp.h>
#endif

static bool _lastGlCallHadError = false;

void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

void _glCall(const char* function, const char* file, int line)
{
    _lastGlCallHadError = false;
    while (GLenum error = glGetError())
    {
        _lastGlCallHadError = true;
        const char* errorString = reinterpret_cast<const char*>(gluErrorString(error));
        std::cout << "[OpenGL Error] (" << error << "): "
            << file << " in " << function << " on line " << line
            << "\nGlErrorMessage: " << errorString
            << std::endl;
        //exit(error);
    }

}

bool LastGlCallHadError() { return _lastGlCallHadError; }

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
void _PrintBacktrace()
{
#ifdef _WIN32 
    if (!SymInitialize(GetCurrentProcess(), nullptr, TRUE))
    {
        DWORD dwError = GetLastError();
        std::cerr << "_PrintBacktrace: Failed to initialize. ErrorCode: " << dwError << std::endl;
        return;
    }

    constexpr int MAX_STACK_FRAMES = 64;
    void* stackTraces[MAX_STACK_FRAMES];
    WORD numFrames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, stackTraces, nullptr);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    if (symbol == nullptr)
    {
        std::cerr << "_PrintBacktrace: Failed to allocate memory\n" << std::endl;
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
            std::cerr << "_PrintBacktrace: Failed to get function name from address. ErrorCode = "
                << dwError << "\n" << std::endl;
            break;
        }
        if (std::string(symbol->Name) == "main")
            break;
        functionNames.push_back(std::string(symbol->Name));
    }
    
    for(int i = functionNames.size() - 1; i >= 0; i--)
        std::cerr << "    Function: " << functionNames[i] << std::endl;
    free(symbol);
#endif
}
