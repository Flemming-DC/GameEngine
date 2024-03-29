#pragma once
#include "Engine.h"
#include "CollisionLoop.h"

class Core
{
public:
    static void Run(std::unique_ptr<Scene> firstScene);
    static void StartRunningGame(); // used by engine and editor, not the game itself
    static void StopRunningGame();
    static void MarkAsEditor() { inEditor = true; }

private:
    static bool gameIsRunning;
    static bool inEditor;

    static void Setup();
    static void Update();
    static void Shutdown();
    static void LogHeader(std::string header) { logger::print("------- " + header + " --------"); }
};
