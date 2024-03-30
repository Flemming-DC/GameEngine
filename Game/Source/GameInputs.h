#pragma once
#include "Engine.h"
#include "InputAction.h"

class GameInputs : public Dynamic
{
public:
	inline static InputAction<bool>& Jump() { return *jump; };
	inline static InputAction<float>& Fire() { return *fire; };
	inline static InputAction<glm::vec2>& Walk() { return *walk; };


	static InputAction<bool>* jump;
	static InputAction<float>* fire;
	static InputAction<glm::vec2>* walk;

private:

	void OnGameStart() override;
	void OnGameEnd() override;

};