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
#include "NamingSaver.h"


void Core::Run(std::string firstScenePath)
{
    logger::print("------- Setup --------");
    Setup(firstScenePath);

    logger::print("------- Loop --------");
    while (!EngineMode::ShouldClose())
        Update();

    logger::print("------- Shutdown --------");
    Shutdown();
    logger::print("------- Done --------");
}


// ------------------ setup, Update, Shutdown --------------------

void Core::Setup(std::string firstScenePath)
{
    // externals setup
    OpenGlSetup::Setup();
    InputLoop::GlfwSetup();
    ImGuiSetup::Setup();
    // engine setup
    NamingSaver::Load();
    Time::Setup();
    CollisionLoop::Setup();
    EngineAssets::Setup();
    Entity::DeclareComps<Transform, Renderable, Camera, CircleCollider, RectangleCollider, PolygonCollider>();
    // editor / game setup
    if (EngineMode::InEditor())
        Dynamic::CallOnEditorStart();
    Dynamic::CallOnGameStart(); // this must be called even if (gameIsRunning == false). Thats a bit messy.
    Scene::ActivateImmediately(firstScenePath);



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
    Entity::DestroyTheDoomed();
    if (EngineMode::GameCloseButtonIsClicked())
        StopRunningGame();
    Delay::Update(); 

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
    if (EngineMode::InEditor())
        NamingSaver::Save(); // namings cant change at playtime
    // externals
    ImGuiSetup::Shutdown();
    OpenGlSetup::Shutdown();
}



// ---------------------- open/close game ----------------------


void Core::StartRunningGame()
{
    logger::print("--- Running Game ---");
    EngineMode::gameIsRunning = true;
    EngineMode::MarkGameCloseButtonAsUnclicked();
    Time::GameSetup();
    Dynamic::CallOnGameStart();
    Scene::Save();
    Scene::ReloadImmediately();
    Renderer::ShowWindow(true);
}


void Core::StopRunningGame()
{
    Delay::ToFrameEnd([]()
        {
            Renderer::ShowWindow(false); 
            Scene::ReloadImmediately();
            Dynamic::CallOnGameEnd();
            EngineMode::gameIsRunning = false;
            EngineMode::MarkGameCloseButtonAsUnclicked();
        });
    logger::print("--- Stopping Game ---");
}
