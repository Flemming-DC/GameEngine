#pragma once
#include "InputAction.h"
#include "Dynamic.h"


namespace Editor
{
	class EditorInputs : public Dynamic
	{
	public:
		inline static InputAction<float>& Zoom() { return *zoom; };
		inline static InputAction<glm::vec2>& MoveCamera() { return *moveCamera; };

	private:
		static InputAction<float>* zoom;
		static InputAction<glm::vec2>* moveCamera;

		void OnGameStart() override;
	};
}