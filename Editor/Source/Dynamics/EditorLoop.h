#pragma once
#include "Dynamic.h"

namespace Editor
{
	class EditorLoop : public Dynamic
	{
	public:
		//static void OnSceneChanged();

	private:
		void OnEditorStart() override;
		void OnEditorUpdate() override;

	};
}

