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
    // We set the gl version to 3.3, since the shaders won't accept smaller versions and old pc's won't accept higher versions.
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // locking version
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    bool enableVsync = true; // Vsync = sync render update loop to monitor refresh rate.
    glfwSwapInterval(enableVsync ? 1 : 0);

    // print info
    glCall(auto glVersion = glGetString(GL_VERSION));
    std::cout << "glVersion = " << glVersion << std::endl;
    // apply features
    glCall(glEnable(GL_BLEND)); // enable blending fragment color values  for use by transperency
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glfwSetFramebufferSizeCallback(OpenGlSetup::GetWindow(), FramebufferSizeCallback);
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
    bool close = glfwWindowShouldClose(window);
    return !close;
}

std::pair<int, int> OpenGlSetup::WindowSize()
{
    int width = -1;
    int height = -1;
    glfwGetWindowSize(window, &width, &height);
    Deny(width <= 0 || height <= 0,
        "impossible window size");
    return {width, height};
}


// get pair<major version, minor version>
std::pair<int, int> OpenGlSetup::GlVersionPair()
{
    int major, minor;
    glCall(glGetIntegerv(GL_MAJOR_VERSION, &major));
    glCall(glGetIntegerv(GL_MINOR_VERSION, &minor));
    return { major, minor };
}

void OpenGlSetup::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glCall(glViewport(0, 0, width, height));
}
