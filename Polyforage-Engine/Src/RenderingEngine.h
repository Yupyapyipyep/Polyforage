#pragma once

#include "RenderContext.h"

class RenderingEngine
{
public:
	static void BeginFrame();
	static void EndFrame();
	static void RenderFrame();
	static void Present();
	static void SetViewProjection(const m4& view, const m4& projection);
	static void SubmitRenderable(Renderable renderable, const m4& transform);
};