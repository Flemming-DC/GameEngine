#include "ErrorChecker.h"
#include <iostream>
#include <string>
#include <GL/glu.h>

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

