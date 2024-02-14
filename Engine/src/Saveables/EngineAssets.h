#pragma once
#include "Shader.h"
#include <stduuid/uuid.h>
#include <optional>

class EngineAssets
{
public:
	static void Setup();
	static const Shader& SolidColorShader() { return solidColorShader; }

private:
	static Shader solidColorShader; // used by gizmo
};


