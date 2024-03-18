#pragma once
#include "ErrorChecker.h"

#define glCall(codeLine) OpenGlError::_glClearError();\
    codeLine;\
    OpenGlError::_glCall(#codeLine, __FILE__, __LINE__)


namespace OpenGlError
{

    void _glClearError();
    void _glCall(const char* function, const char* file, int line);
    bool _LastGlCallHadError();

}






