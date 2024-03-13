#pragma once
#include "Engine.h"
#include "CollisionLoop.h"

namespace Core
{

    template <typename SceneType>
    void Run()
    {
        Log("--------- Setup --------- ");
        Initializer::Setup();
        Dynamic::CallOnEngineStart();
        Dynamic::CallOnGameStart();
        Scene::Activate<SceneType>();

        Log("--------- Loop --------- ");
        while (Initializer::NewFrame())
        {
            CollisionLoop::Update();
            Dynamic::CallOnUpdate();
            Entity::CallOnUpdate();
            Renderer::DrawToScreen();
            Delay::CallToFrameEnd();
            Initializer::EndFrame();
        }

        Log("------- Shutdown --------");
        Scene::GetActiveScene().ShutDown();
        Dynamic::CallOnGameEnd();
        Dynamic::CallOnEngineEnd();
        Initializer::Shutdown();

        Log("------- Done --------");
    }


}