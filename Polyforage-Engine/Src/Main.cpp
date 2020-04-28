#include <iostream>
#include "Display.h"
#include "RenderContext.h"
#include "Model.h"
#include "Input.h"
#include "RenderingEngine.h"
#include "Transform.h"

struct Camera
{
	v3 Pos;
	quat Rot;
	r32 Fov;
	r32 Speed;
};

static Camera S_Camera;

static void Movement(r32 dt)
{
	if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
		Input::SetCursorVisible(!Input::IsCursorVisible());
	
	if (Input::IsCursorVisible())
		return;

	if (Input::IsKeyDown(GLFW_KEY_W))
		S_Camera.Pos += S_Camera.Rot.GetForward() * S_Camera.Speed * dt;
	if(Input::IsKeyDown(GLFW_KEY_S))
		S_Camera.Pos += S_Camera.Rot.GetBack() * S_Camera.Speed * dt;
	if(Input::IsKeyDown(GLFW_KEY_A))
		S_Camera.Pos += S_Camera.Rot.GetLeft() * S_Camera.Speed * dt;
	if (Input::IsKeyDown(GLFW_KEY_D))
		S_Camera.Pos += S_Camera.Rot.GetRight() * S_Camera.Speed * dt;
		
	u32 width, height;
	Display::GetSize(&width, &height);
	v2 CENTER(width / 2, height / 2);

	v2 deltaPos = CENTER - Input::GetMouseCursorPos();
	b32 rotX = deltaPos.y != 0;
	b32 rotY = deltaPos.x != 0;

	if (rotX)
		S_Camera.Rot = (quat(S_Camera.Rot.GetRight(), S_Camera.Speed * deltaPos.y * dt) * S_Camera.Rot).Normalized();
	if(rotY)
		S_Camera.Rot = (quat(v3(0.0f, 1.0f, 0.0f), S_Camera.Speed * -deltaPos.x * dt) * S_Camera.Rot).Normalized();
	if (rotX || rotY)
		Input::SetMouseCursorPos(CENTER);
}

int main()
{
	S_Camera.Pos = v3(0.0f, 0.0f, 0.0f);
	S_Camera.Rot = quat(0.0f, 0.0f, 0.0f, 1.0f);
	S_Camera.Fov = 70.0f;
	S_Camera.Speed = 2.0f;
	
	Display::InitAndCreate(800, 600);
	RenderContext::Init();

	Model loadedModel;
	ModelLoader::LoadPFMDL("Res/Models/cube.pfmdl", &loadedModel);

	Renderable cube = RenderContext::CreateRenderable3D(loadedModel);

	Model triangleModel;
	triangleModel.Vertices = { Vertex(v3(-0.75f, -0.75f, 0.0f), v3(1.0f, 0.0f, 0.0f)),
							   Vertex(v3(0.0f, 0.75f, 0.0f), v3(0.0f, 1.0f, 0.0f)),
							   Vertex(v3(0.75f, -0.75f, 0.0f), v3(0.0f, 0.0f, 1.0f))};
	triangleModel.Indices = {0, 1, 2};

	Renderable triangle = RenderContext::CreateRenderable3D(triangleModel);
	
	Input::InitInput();
	b32 Running = true;

	Transform transform;
	transform.Translate(v3(0.0f, 0.0f, 0.0f));
	while (Running)
	{
		if (!Display::IsCloseRequested())
		{
			Running = false;
		}

		u32 width, height;
		Display::GetSize(&width, &height);
		m4 view = S_Camera.Rot.Conjugate().CreateRotationMatrix() * m4::CreateTranslation(S_Camera.Pos * -1.0f);
		m4 projection = m4::CreatePerspective(width, height, S_Camera.Fov, 0.1f, 1000.0f);

		RenderingEngine::BeginFrame();
		RenderingEngine::SetViewProjection(view, projection);
		RenderingEngine::SubmitRenderable(cube, transform.CreateTransformMatrix());
		RenderingEngine::EndFrame();
		RenderingEngine::RenderFrame();
		RenderingEngine::Present();

		Movement(1.0f / 60.0f);

		Input::Update();
		
	}

	Display::Destroy();
}