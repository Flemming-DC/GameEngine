#pragma once
#include "Engine.h"
#include "CollisionLoop.h"

class Core
{
public:

    template <typename SceneType>
    static void Run()
    {
        logger::print("--------- Setup --------- ");
        Setup();
        Scene::Activate<SceneType>();

        logger::print("--------- Loop --------- ");
        while (OpenGlSetup::NewFrame())
            Update();

        logger::print("------- Shutdown --------");
        Shutdown();

        logger::print("------- Done --------");
    }

private:
    static void Setup();
    static void Update();
    static void Shutdown();
};
