#include "InputLoop.h"
#include "GlfwInput.h"
#include "InputAction.h" // its a bit messy that input imports this, since InputAction is a higher level objects than Input




void InputLoop::GlfwSetup() { GlfwInput::Setup(); }

void InputLoop::LateUpdate()
{
    InputAction<bool>::LateUpdate();
    InputAction<float>::LateUpdate();
    InputAction<glm::vec2>::LateUpdate();
    GlfwInput::LateUpdate(); // this update call must happen in the end
}


