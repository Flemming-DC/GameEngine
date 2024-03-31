#include "EditorCore.h"
#include "EngineMode.h"
#include "Dynamic.h"
#include "EditorLoop.h"


void EditorCore::MarkAsEditor()
{
	EngineMode::MarkAsEditor();
	Dynamic::Setup<EditorLoop>();
}

