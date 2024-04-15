#pragma once
#include "GlmTools.h"
#include "ShortHands.h"

// wasd / arrows
// drag
// switch pos, rot, scale
// later on switch filter
namespace Editor
{
	class SelectionMover
	{
		Shorts;
	public:

	private:
		static void SetPosition(vec2 newPos);
		static void SetRotation(float angle);
		static void SetScale(vec2 newScale);
	};
}

