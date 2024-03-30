#pragma once
#include "Engine.h"

class Core
{
public:
    static void Run(std::unique_ptr<Scene> firstScene);

    static void MarkAsEditor();
    static void StartRunningGame(); // used by engine and editor, not the game itself
    static void StopRunningGame();

private:
    static void Setup();
    static void Update();
    static void Shutdown();
    static void LogHeader(std::string header) { logger::print("------- " + header + " --------"); }

};
