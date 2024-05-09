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
		static void Update();

	private:
		static void UpdateControl();
		static vec2 Delta();
		static void Move(vec2 delta);
		/*
		static void SetPosition(vec2 newPos);
		static void SetRotation(float angle);
		static void SetScale(vec2 newScale);
		*/
	};
}

