#include "EditorCore.h"
#include "Core.h"
#include "Dynamic.h"
#include "Editor.h"


void EditorCore::MarkAsEditor()
{
	Core::MarkAsEditor();
	Dynamic::Setup<Editor>();
}

