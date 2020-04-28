#pragma once

#include "GameDefines.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Display
{
public:
	static void InitAndCreate(u32 width, u32 height);
	static void Destroy();
	static b32 IsCloseRequested();
	static void Update();

	static void GetSize(u32* width, u32* height);

	static GLFWwindow* GetHandle();

};

