#pragma once
#include "Engine.h"
#include "InputAction.h"

class GameInputs : public Dynamic
{
public:
	static InputAction<bool>* jumpPtr;

	static InputAction<bool>& jump;
	static InputAction<float>& fire;
	static InputAction<glm::vec2>& walk;

private:

	void OnGameStart() override;
	void OnGameEnd() override;

};