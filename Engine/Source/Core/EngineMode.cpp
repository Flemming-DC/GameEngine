#include "EngineMode.h"
#include "OpenGlSetup.h"

bool EngineMode::gameIsRunning = true;
bool EngineMode::inEditor = false;
bool EngineMode::exitEditorhasBeenCalled = false;


void EngineMode::MarkGameWindowAsUnclosed() { glCall(glfwSetWindowShouldClose(OpenGlSetup::GetWindow(), false)); }
bool EngineMode::CloseButtonIsClicked() { return glfwWindowShouldClose(OpenGlSetup::GetWindow()); }

void EngineMode::MarkAsEditor()
{
    inEditor = true;
    gameIsRunning = false;
}
