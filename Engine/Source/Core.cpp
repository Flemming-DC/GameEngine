#include "Core.h"
#include "InputLoop.h"
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
    InputLoop::Setup();
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
    // setup frame
    ImGuiSetup::EarlyUpdate();
    Time::Update();
    
    // update world
    CollisionLoop::Update();
    Dynamic::CallOnUpdate();
    Entity::Update();
    Renderer::DrawToScreen();
    Delay::CallToFrameEnd();

    // shutdown frame
    InputLoop::LateUpdate();
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
