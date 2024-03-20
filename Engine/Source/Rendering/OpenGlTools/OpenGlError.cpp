#include "OpenGlError.h"
#include "OpenGlExternal.h"
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
        while (unsigned int errorCode = glGetError())
        {
            _lastGlCallHadError = true;
            auto message = "OpenGL Error [" + std::to_string(errorCode) + "]: " + _ErrorString(errorCode) + "\n";
            ErrorChecker::_RaiseError(message, function, file, line);
        }

    }

    bool _LastGlCallHadError() { return _lastGlCallHadError; }

    std::string _ErrorString(unsigned int errorCode)
    {
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            return (std::string)"GL_INVALID_ENUM: \n"
                + "This error is given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; "
                + "if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, "
                + "then GL_INVALID_OPERATION is the result instead.";
        case GL_INVALID_VALUE:
            return (std::string)"GL_INVALID_VALUE: \n"
                + "This error is given when a value parameter is not a legal value for that function.This is only given for local problems; "
                + "if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, "
                + "then GL_INVALID_OPERATION is the result instead.";
        case GL_INVALID_OPERATION:
            return (std::string)"GL_INVALID_OPERATION: \n"
                + "This error is given when the set of state for a command is not legal for the parameters given to that command. "
                + "It is also given for commands where combinations of parameters define what the legal parameters are.";
        case GL_STACK_OVERFLOW:
            return (std::string)"GL_STACK_OVERFLOW: \n"
                + "This error is given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size.";
        case GL_STACK_UNDERFLOW:
            return (std::string)"GL_STACK_UNDERFLOW: \n"
                + "This error is given when a stack popping operation cannot be done because the stack is already at its lowest point.";
        case GL_OUT_OF_MEMORY:
            return (std::string)"GL_OUT_OF_MEMORY: \n"
                + "This error is given when performing an operation that can allocate memory, and the memory cannot be allocated. "
                + "The results of OpenGL functions that return this error are undefined; "
                + "it is allowable for partial execution of an operation to happen in this circumstance.";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return (std::string)"GL_INVALID_FRAMEBUFFER_OPERATION: \n"
                + "This error is given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.";
        case GL_CONTEXT_LOST:
            return (std::string)"GL_CONTEXT_LOST: \n"
                + "This error is given if the OpenGL context has been lost, due to a graphics card reset.";
        case GL_NO_ERROR:
            return (std::string)"GL_NO_ERROR: \n";
        default:
            return std::to_string(errorCode) + " (failed to cast errorCode into errorString)";
        }
    }
}

