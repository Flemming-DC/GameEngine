#pragma once
#include "Engine.h"


class Editor : public Dynamic
{
public:
	//static void OnSceneChanged();


private:
	void OnEditorStart() override { Renderer::ShowWindow(false); }
	void OnUpdate() override;
};

