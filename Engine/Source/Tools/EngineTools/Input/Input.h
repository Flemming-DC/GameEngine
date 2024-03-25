#pragma once
#include "KeyMap.h"


class Input
{
public:
	static void Setup();
	static void LateUpdate();

	static bool KeyPressed(Keyboard key);
	static bool KeyReleased(Keyboard key);
	static bool KeyHeldDown(Keyboard key);
	static bool GetScrollHeldDown();
	static int GetScrollDirection();
	

};

