#include "Core.h"
#include "Input.h"
#include "Time_.h"
#include "EngineAssets.h"
#include "Renderer.h"
#include "Scene.h"
#include "Dynamic.h"
#include "CollisionLoop.h"
#include "ImGuiSetup.h"


void Core::Setup()
{
    // externals setup
    OpenGlSetup::Setup();
    ImGuiSetup::Setup();
    // engine setup
    Input::Setup();
    Time::Setup();
    CollisionLoop::Setup();
    EngineAssets::Setup();
    // editor / game setup
    Dynamic::CallOnEditorStart();
    Dynamic::CallOnGameStart();
    // Scene::Activate<SceneType>();
}

void Core::Update()
{
    ImGuiSetup::EarlyUpdate();
    Time::Update();
    
    CollisionLoop::Update();
    Dynamic::CallOnUpdate();
    Entity::CallOnUpdate();
    Renderer::DrawToScreen();

    Delay::CallToFrameEnd();
    Input::Update(); // resetting input data, must happen before glfwPollEvents
    ImGuiSetup::LateUpdate();
}

void Core::Shutdown()
{
    // editor / game shutdown
    Scene::GetActiveScene().ShutDown();
    Dynamic::CallOnGameEnd();
    Dynamic::CallOnEditorEnd();
    // engine shutdown
    Renderer::ShutDown();
    Dynamic::Clear();
    // externals
    ImGuiSetup::Shutdown();
    OpenGlSetup::Shutdown();
}
