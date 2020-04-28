#pragma once

#include "Types.h"
#include <GL\glew.h>
#include "Model.h"

enum Shader
{
	Shader_Test,

	Num_Shaders,
	Shader_Invalid
};

typedef u32 Renderable;

class RenderContext
{
public:
	static void Init();
	static void Clear();

	static void BindShader(Shader shader);
	static void SetUniform(const std::string& name, r32 value);
	static void SetUniform(const std::string& name, const v2& value);
	static void SetUniform(const std::string& name, const v3& value);
	static void SetUniform(const std::string& name, const m4& value);

	static Renderable CreateRenderable3D(const Model& model);
	static void Render(Renderable renderable);
};
