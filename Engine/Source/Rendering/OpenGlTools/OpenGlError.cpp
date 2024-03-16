#include "OpenGlError.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <string>

namespace OpenGlError
{
    static bool _lastGlCallHadError = false;


    void _glClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    void _glCall(const char* function, const char* file, int line)
    {
        _lastGlCallHadError = false;
        while (GLenum errorCode = glGetError())
        {
            _lastGlCallHadError = true;
            const char* errorString = reinterpret_cast<const char*>(gluErrorString(errorCode));
            auto message = "OpenGL Error [" + std::to_string(errorCode) + "]: " + errorString;
            ErrorChecker::_RaiseError(message, function, file, line);
        }

    }

    bool _LastGlCallHadError() { return _lastGlCallHadError; }
}

