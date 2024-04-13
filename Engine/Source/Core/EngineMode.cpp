#include "EngineMode.h"
#include "OpenGlSetup.h"

bool EngineMode::gameIsRunning = true;
bool EngineMode::inEditor = false;
bool EngineMode::exitEditorhasBeenCalled = false;


void EngineMode::MarkGameCloseButtonAsUnclicked() { glCall(glfwSetWindowShouldClose(OpenGlSetup::GetWindow(), false)); }
bool EngineMode::GameCloseButtonIsClicked() { return glfwWindowShouldClose(OpenGlSetup::GetWindow()); }

void EngineMode::MarkAsEditor()
{
    inEditor = true;
    gameIsRunning = false;
}
