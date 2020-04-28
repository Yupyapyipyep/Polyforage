#include "Input.h"
#include <cstring>
#include "Display.h"

struct Input_Data
{
	GLFWwindow* Window;
	b32 IsCursorVisible;
	bool Keys[GLFW_KEY_LAST];
	bool LastKeys[GLFW_KEY_LAST];
	bool MouseButtons[GLFW_MOUSE_BUTTON_LAST];
	bool LastMouseButtons[GLFW_MOUSE_BUTTON_LAST];
};

static Input_Data S_Data;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		S_Data.Keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		S_Data.Keys[key] = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		S_Data.MouseButtons[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		S_Data.MouseButtons[button] = false;
	}
}

void Input::InitInput()
{
	glfwSetKeyCallback(Display::GetHandle(), key_callback);
	glfwSetMouseButtonCallback(Display::GetHandle(), mouse_button_callback);
	memset(S_Data.Keys, 0, GLFW_KEY_LAST);
	memset(S_Data.LastKeys, 0, GLFW_KEY_LAST);
	memset(S_Data.MouseButtons, 0, GLFW_MOUSE_BUTTON_LAST);
	memset(S_Data.LastMouseButtons, 0, GLFW_MOUSE_BUTTON_LAST);
	S_Data.IsCursorVisible = true;
	S_Data.Window = Display::GetHandle();
}

void Input::Update()
{
	memcpy(S_Data.LastKeys, S_Data.Keys, GLFW_KEY_LAST);
	memcpy(S_Data.LastMouseButtons, S_Data.MouseButtons, GLFW_MOUSE_BUTTON_LAST);
}

b32 Input::IsKeyDown(u32 key)
{
	return S_Data.Keys[key];
}

b32 Input::IsKeyPressed(u32 key)
{
	return S_Data.Keys[key] && !S_Data.LastKeys[key];
}

b32 Input::IsKeyReleased(u32 key)
{
	return !S_Data.Keys[key] && S_Data.LastKeys[key];
}

b32 Input::IsMouseButtonDown(u32 button)
{
	return S_Data.MouseButtons[button];
}

b32 Input::IsMouseButtonPressed(u32 button)
{
	return S_Data.MouseButtons[button] && !S_Data.LastMouseButtons;
}

b32 Input::IsMouseButtonReleased(u32 button)
{
	return !S_Data.MouseButtons[button] && S_Data.LastMouseButtons[button];
}

b32 Input::IsCursorVisible()
{
	return S_Data.IsCursorVisible;
}

void Input::SetCursorVisible(b32 visible)
{
	glfwSetInputMode(S_Data.Window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	S_Data.IsCursorVisible = visible;
}

void Input::SetMouseCursorPos(const v2& pos)
{
	glfwSetCursorPos(S_Data.Window, pos.x, pos.y);
}

v2 Input::GetMouseCursorPos()
{
	double x_d, y_d;
	glfwGetCursorPos(S_Data.Window, &x_d, &y_d);
	return v2(x_d, y_d);
}
