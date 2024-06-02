#include "OpenGlSetup.h"
#include "OpenGlError.h"
#include "OpenGlExternal.h"

bool OpenGlSetup::openGLInitialized = false;
GLFWwindow* OpenGlSetup::window = nullptr;
std::pair<int, int> OpenGlSetup::lastWindowSize = { 960, 720 };

void glfwErrorCallback(int errorCode, const char* errorMessage)
{
    std::cout << "errorCode [" << errorCode << "] message: " << errorMessage << std::endl;
}

void OpenGlSetup::Setup()
{
    glfwSetErrorCallback(glfwErrorCallback);

    // glfw setup
    if (!glfwInit())
        RaiseError("glfwInit failed"); 
    // 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // locking version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(lastWindowSize.first, lastWindowSize.second, "Default Title", NULL, NULL); 
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

void OpenGlSetup::ShutDown()
{
    glfwTerminate();
}

bool OpenGlSetup::Update()
{
    lastWindowSize = WindowSize();
    glCall(glfwSwapBuffers(window)); // Swap front and back buffers
    glCall(glfwPollEvents()); // Poll for and process events
    return !glfwWindowShouldClose(window);
}

std::pair<int, int> OpenGlSetup::WindowSize()
{
    int width = -1;
    int height = -1;
    glfwGetWindowSize(window, &width, &height);
    if (width <= 0 || height <= 0)
        RaiseError("impossible window size");
    return {width, height};
}