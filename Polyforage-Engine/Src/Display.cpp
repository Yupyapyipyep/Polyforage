#include "Display.h"

static GLFWwindow* S_Window;

b32 ShouldDisplayClose()
{
	return glfwWindowShouldClose(S_Window);
}

void Display::InitAndCreate(u32 width, u32 height)
{
	if (!glfwInit())
	{
		//TODO
	}

	S_Window = glfwCreateWindow(width, height, GAME_TITLE, 0, 0);
	if (!S_Window)
	{
		//TODO
	}

	glfwMakeContextCurrent(S_Window);

	//vsync
	glfwSwapInterval(1);
}

void Display::Destroy()
{
	glfwDestroyWindow(S_Window);
	glfwTerminate();
}

b32 Display::IsCloseRequested()
{
	return !glfwWindowShouldClose(S_Window);
}

void Display::Update()
{
	glfwSwapBuffers(S_Window);
	glfwPollEvents();
}

void Display::GetSize(u32* width, u32* height)
{
	int w, h;
	glfwGetWindowSize(S_Window, &w, &h);
	*width = w;
	*height = h;
}

GLFWwindow* Display::GetHandle()
{
	return S_Window;
}
