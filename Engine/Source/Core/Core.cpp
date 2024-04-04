#include "Core.h"
#include "InputLoop.h"
#include "Time_.h"
#include "EngineAssets.h"
#include "Renderer.h"
#include "Scene.h"
#include "Dynamic.h"
#include "CollisionLoop.h"
#include "ImGuiSetup.h"
#include "EngineMode.h"
#include "OpenGlSetup.h"



void Core::Run(std::unique_ptr<Scene> firstScene)
{
    LogHeader("Setup");
    Scene::SetFirstScene(std::move(firstScene));
    Setup();

    LogHeader("Loop");
    while (!EngineMode::ShouldClose())
        Update();

    LogHeader("Shutdown");
    Shutdown();
    LogHeader("Done");
}


// ------------------ setup, Update, Shutdown --------------------

void Core::Setup()
{
    // externals setup
    OpenGlSetup::Setup();
    InputLoop::GlfwSetup();
    ImGuiSetup::Setup();
    // engine setup
    Time::Setup();
    CollisionLoop::Setup();
    EngineAssets::Setup();
    // editor / game setup
    if (EngineMode::InEditor())
        Dynamic::CallOnEditorStart();
    Dynamic::CallOnGameStart(); // this must be called even if (gameIsRunning == false). Thats a bit messy.
    Scene::ReloadImmediately();
}


void Core::Update()
{
    // setup frame
    OpenGlSetup::Update();
    ImGuiSetup::EarlyUpdate();
    Time::Update();

    // update world
    if (EngineMode::GameIsRunning())
    {
        CollisionLoop::Update();
        Dynamic::CallOnUpdate();
        Entity::Update();
        Renderer::DrawToScreen();
    }
    if (EngineMode::InEditor())
        Dynamic::CallOnEditorUpdate();
    if (EngineMode::CloseButtonIsClicked())
        StopRunningGame();
    Delay::CallToFrameEnd(); 

    // shutdown frame
    InputLoop::LateUpdate();
    ImGuiSetup::LateUpdate();
}


void Core::Shutdown()
{
    // editor / game shutdown
    Scene::GetActiveScene().ShutDown(); // inEditor on end game reload scene
    if (EngineMode::GameIsRunning())
        Dynamic::CallOnGameEnd(); // evt. call EndGame
    if (EngineMode::InEditor())
        Dynamic::CallOnEditorEnd();
    // engine shutdown
    Renderer::ShutDown();
    Dynamic::Clear();
    // externals
    ImGuiSetup::Shutdown();
    OpenGlSetup::Shutdown();
}



// ---------------------- open/close game ----------------------


void Core::StartRunningGame()
{
    EngineMode::gameIsRunning = true;
    Time::GameSetup();
    Dynamic::CallOnGameStart();
    EngineMode::MarkGameWindowAsUnclosed();
    Renderer::ShowWindow(true); 
}


void Core::StopRunningGame()
{
    Delay::ToFrameEnd([]()
        {
            Renderer::ShowWindow(false); 
            //Scene::ReloadImmediately();
            Dynamic::CallOnGameEnd();
            EngineMode::gameIsRunning = false;
        });
}
