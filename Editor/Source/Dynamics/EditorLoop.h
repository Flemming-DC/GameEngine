#pragma once
#include "Dynamic.h"

class EditorLoop : public Dynamic
{
public:
	//static void OnSceneChanged();

private:
	void OnEditorStart() override;
	void OnEditorUpdate() override;

};

