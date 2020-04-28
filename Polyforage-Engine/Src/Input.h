#pragma once

#include "Math.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Input
{
public:
	static void InitInput();
	static void Update();

	static b32 IsKeyDown(u32 key);
	static b32 IsKeyPressed(u32 key);
	static b32 IsKeyReleased(u32 key);

	static b32 IsMouseButtonDown(u32 button);
	static b32 IsMouseButtonPressed(u32 button);
	static b32 IsMouseButtonReleased(u32 button);

	static b32 IsCursorVisible();
	static void SetCursorVisible(b32 visible);

	static void SetMouseCursorPos(const v2& pos);
	static v2 GetMouseCursorPos();
};