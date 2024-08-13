#pragma once
#include "ErrorChecker.h"

#define glCall(codeLine) \
    InDebug(OpenGlError::_glClearError());\
    codeLine;\
    InDebug(OpenGlError::_glCall(#codeLine, __FILE__, __LINE__))


namespace OpenGlError
{

    void _glClearError();
    void _glCall(const char* function, const char* file, int line);
    bool _LastGlCallHadError();
    std::string _ErrorString(unsigned int errorCode);
}






