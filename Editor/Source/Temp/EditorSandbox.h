#pragma once
#include "Dynamic.h"

namespace Editor
{
	class EditorSandbox : public Dynamic
	{
	public:
		//static void OnSceneChanged();

	private:
		void OnEditorStart() override;
		void OnEditorUpdate() override;

	};
}



