#include "Core.h"
#include "InputLoop.h"
#include "Time_.h"
#include "EngineAssets.h"
#include "Renderer.h"
#include "Scene.h"
#include "Dynamic.h"
#include "CollisionLoop.h"
#include "ImGuiSetup.h"

bool Core::gameIsRunning = true;
bool Core::inEditor = false;


void Core::Run(std::unique_ptr<Scene> firstScene)
{
    LogHeader("Setup");
    Scene::SetFirstScene(std::move(firstScene));
    Setup();

    LogHeader("Loop");
    while (OpenGlSetup::Update())
        Update();

    LogHeader("Shutdown");
    Shutdown();
    LogHeader("Done");
}

void Core::Setup()
{
    // externals setup
    OpenGlSetup::Setup();
    ImGuiSetup::Setup(inEditor);
    // engine setup
    InputLoop::Setup();
    Time::Setup();
    CollisionLoop::Setup();
    EngineAssets::Setup();
    // editor / game setup
    if (inEditor)
        Dynamic::CallOnEditorStart();
    Dynamic::CallOnGameStart(); // this must be called even if (gameIsRunning == false). Thats a bit messy.
    Scene::ReloadImmediately();
}


void Core::Update()
{
    // setup frame
    ImGuiSetup::EarlyUpdate();
    Time::Update();
    
    // update world
    if (gameIsRunning)
    {
        CollisionLoop::Update();
        Dynamic::CallOnUpdate();
        Entity::Update();
        Renderer::DrawToScreen();
    }
    if (inEditor)
        Dynamic::CallOnEditorUpdate();
    Delay::CallToFrameEnd(); 

    // shutdown frame
    InputLoop::LateUpdate();
    ImGuiSetup::LateUpdate();
}


void Core::Shutdown()
{
    // editor / game shutdown
    Scene::GetActiveScene().ShutDown(); // inEditor on end game reload scene
    if (gameIsRunning)
        Dynamic::CallOnGameEnd(); // evt. call EndGame
    if (inEditor)
        Dynamic::CallOnEditorEnd();
    // engine shutdown
    Renderer::ShutDown();
    Dynamic::Clear();
    // externals
    ImGuiSetup::Shutdown();
    OpenGlSetup::Shutdown();
}


void Core::StartRunningGame()
{
    gameIsRunning = true;
    Time::GameSetup();
    Dynamic::CallOnGameStart();
    Renderer::ShowWindow(true);
}


void Core::StopRunningGame()
{
    Delay::ToFrameEnd([]()
        {
            Renderer::ShowWindow(false);
            Scene::ReloadImmediately();
            Dynamic::CallOnGameEnd();
            gameIsRunning = false;
        });
}

void Core::MarkAsEditor()
{
    inEditor = true;
    gameIsRunning = false;
}