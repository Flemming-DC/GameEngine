#pragma once
#include "Engine.h"

class Core
{
public:
    static void Run(std::string firstScenePath);

    static void StartRunningGame(); // used by engine and editor, not the game itself
    static void StopRunningGame();

private:
    static void Setup(std::string firstScenePath);
    static void Update();
    static void Shutdown();

};
