#pragma once
#include "Engine.h"
#include "CollisionLoop.h"

class Core
{
public:
    template <typename SceneType>
    static void Run()
    {
        LogHeader("Setup");
        Setup();
        Scene::Activate<SceneType>();

        LogHeader("Loop");
        while (OpenGlSetup::Update())
            Update();

        LogHeader("Shutdown");
        Shutdown();
        LogHeader("Done");
    }

private:
    static void Setup();
    static void Update();
    static void Shutdown();
    static void LogHeader(std::string header) { logger::print("------- " + header + " --------"); }
};
