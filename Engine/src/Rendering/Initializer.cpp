#include "Initializer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ErrorChecker.h"
#include "Input.h"
#include "Time_.h"
#include "EngineAssets.h"
#include "Renderer.h"

bool Initializer::openGLInitialized = false;
int Initializer::width = 960;
int Initializer::height = 720;
GLFWwindow* Initializer::window = nullptr;

void Initializer::Setup()
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
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSwapInterval(1);

    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ImGui setup
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();
    io.FontGlobalScale = 1.2f; // font size

    openGLInitialized = true;

    //custom setup
    Input::Setup();
    Time::Setup();
    EngineAssets::Setup();


}

void Initializer::Shutdown()
{
    Renderer::ShutDown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool Initializer::NewFrame()
{
    bool close = glfwWindowShouldClose(window);
    if (close)
        return false;
    glCall(glClear(GL_COLOR_BUFFER_BIT)); // same as renderer.Clear()
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Time::Update();
    return true;
}

void Initializer::EndFrame()
{
    Input::Update(); // resetting input data, must happen before glfwPollEvents
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // handle docking
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glCall(glfwSwapBuffers(window)); // Swap front and back buffers
    glCall(glfwPollEvents()); // Poll for and process events
}

