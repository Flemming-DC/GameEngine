#include "ImGuiSetup.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "OpenGlSetup.h"
#include "EngineMode.h"


void ImGuiSetup::Setup()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    if (EngineMode::InEditor())
    {
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    }
    else
        io.IniFilename = nullptr; // Disable automatic .ini file saving/loading

    auto major_minor = OpenGlSetup::GlVersionPair();
    std::string glVersion = "#version " + std::to_string(major_minor.first) + std::to_string(major_minor.second) + "0";
    ImGui_ImplGlfw_InitForOpenGL(OpenGlSetup::GetWindow(), true);
    ImGui_ImplOpenGL3_Init(glVersion.c_str());
    ImGui::StyleColorsDark();
    io.FontGlobalScale = 1.2f; // font size

}

void ImGuiSetup::EarlyUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void ImGuiSetup::LateUpdate()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // handle docking
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) // the flag is only enabled in editor
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiSetup::ShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
