#include "OpenGlSetup.h"
#include "OpenGlError.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool OpenGlSetup::openGLInitialized = false;
int OpenGlSetup::width = 960;
int OpenGlSetup::height = 720;
GLFWwindow* OpenGlSetup::window = nullptr;

void OpenGlSetup::Setup()
{
    // glfw setup
    if (!glfwInit())
        RaiseError("glfwInit failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // locking version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL); // Create a windowed mode window and its OpenGL context
    if (!window)
    {
        glfwTerminate();
        RaiseError("failed to create window");
    }
    glfwMakeContextCurrent(window); // Make the window's context current

    // glew setup
    if (glewInit() != GLEW_OK)
        RaiseError("glewInit failed");
    glCall(auto glVersion = glGetString(GL_VERSION));
    //Log(glVersion);
    glCall(glfwSwapInterval(1));

    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    openGLInitialized = true;

}

void OpenGlSetup::Shutdown()
{
    glfwTerminate();
}

bool OpenGlSetup::NewFrame()
{
    return !glfwWindowShouldClose(window);
}

void OpenGlSetup::EndFrame()
{
    glCall(glfwSwapBuffers(window)); // Swap front and back buffers
    glCall(glfwPollEvents()); // Poll for and process events
}

void OpenGlSetup::Exit()
{
    glCall(glfwSetWindowShouldClose(window, GLFW_TRUE));
    //std::exit(0);
}
