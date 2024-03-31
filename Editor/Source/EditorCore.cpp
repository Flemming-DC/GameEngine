#include "EditorCore.h"
#include "EngineMode.h"
#include "Dynamic.h"
#include "EditorLoop.h"
#include "EditorInputs.h"
using namespace Editor;


void EditorCore::MarkAsEditor()
{
	EngineMode::MarkAsEditor();
	Dynamic::Setup<EditorLoop, EditorInputs>();
}

