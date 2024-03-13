#pragma once
#include "Engine.h"


class Editor : public Dynamic
{
public:
	//static void OnSceneChanged();


private:
	void OnEngineStart() override { Renderer::ShowWindow(false); }
	void OnUpdate() override;
};

