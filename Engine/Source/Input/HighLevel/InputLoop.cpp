#include "InputLoop.h"
#include "GlfwInput.h"
#include "Screen.h"
#include "InputAction.h" // its a bit messy that input imports this, since InputAction is a higher level objects than Input




void InputLoop::Setup()
{
    GlfwInput::Setup();
}

void InputLoop::LateUpdate()
{
    GlfwInput::LateUpdate();
    Screen::LateUpdate();
    InputAction<bool>::LateUpdate();
    InputAction<float>::LateUpdate();
    InputAction<glm::vec2>::LateUpdate();
}


