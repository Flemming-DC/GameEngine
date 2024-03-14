#pragma once
#include "ErrorChecker.h"

#define glCall(x) OpenGlError::_glClearError();\
    x;\
    OpenGlError::_glCall(#x, __FILE__, __LINE__)


namespace OpenGlError
{

    void _glClearError();
    void _glCall(const char* function, const char* file, int line);
    bool _LastGlCallHadError();

}






