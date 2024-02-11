#include "Initializer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "ErrorChecker.h"
#include "Input.h"
#include "Time_.h"


static GLFWwindow* window;

void Setup()
{
    // glfw setup
    if (!glfwInit())
        RaiseError("glfwInit failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // locking version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(960, 720, "Hello World", NULL, NULL); // Create a windowed mode window and its OpenGL context
    if (!window)
    {
        glfwTerminate();
        RaiseError("failed to create window");
    }
    glfwMakeContextCurrent(window); // Make the window's context current

    // glew setup
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit failed" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSwapInterval(1);

    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ImGui setup
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    //custom setup
    Input::Setup(window);
    Time::Setup();
}

void Shutdown()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool NewFrame()
{
    bool close = glfwWindowShouldClose(window);
    if (close)
        return false;
    glCall(glClear(GL_COLOR_BUFFER_BIT)); // same as renderer.Clear()
    ImGui_ImplGlfwGL3_NewFrame();
    Time::Update();
    return true;
}

void EndFrame()
{
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glCall(glfwSwapBuffers(window)); // Swap front and back buffers
    glCall(glfwPollEvents()); // Poll for and process events
}


