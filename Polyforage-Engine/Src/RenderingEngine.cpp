#include "RenderingEngine.h"
#include "Display.h"
#include <vector>

struct RenderingEngine_Renderable
{
	Renderable Renderable;
	m4 Transform;
};

struct RenderingEngine_Data
{
	std::vector<RenderingEngine_Renderable> Renderables;
	m4 ViewProjection;
};

static RenderingEngine_Data S_Data;

void RenderingEngine::BeginFrame()
{
	S_Data.Renderables.clear();
	RenderContext::Clear();
	RenderContext::BindShader(Shader_Test);
}

void RenderingEngine::EndFrame()
{
	RenderContext::BindShader(Shader_Test);
	RenderContext::SetUniform("ViewProjection", S_Data.ViewProjection);
}

void RenderingEngine::RenderFrame()
{
	for (u32 i = 0; i < S_Data.Renderables.size(); i++)
	{
		RenderContext::SetUniform("Model",S_Data.Renderables[i].Transform);
		RenderContext::Render(S_Data.Renderables[i].Renderable);
	}
}

void RenderingEngine::Present()
{
	Display::Update();
}

void RenderingEngine::SetViewProjection(const m4& view, const m4& projection)
{
	S_Data.ViewProjection = projection * view;
	RenderContext::SetUniform("ViewProjection", S_Data.ViewProjection);
}

void RenderingEngine::SubmitRenderable(Renderable renderable, const m4& transform)
{
	RenderingEngine_Renderable r;
	r.Renderable = renderable;
	r.Transform = transform;
	S_Data.Renderables.push_back(r);
}
