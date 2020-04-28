#include "RenderContext.h"
#include <GL\glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

struct RenderContext_Shader
{
	GLuint Program;
	GLuint VertexShader;
	GLuint FragmentShader;
	std::map<std::string, u32> UniformStringMap;
};

struct Renderable3D_Data
{
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	u32 count;
};

struct RenderContetxt_Data
{
	RenderContetxt_Data() :
		BoundShader(Shader_Invalid)
	{}

	RenderContext_Shader Shaders[Num_Shaders];
	Shader BoundShader;
	std::vector<Renderable3D_Data> Renderables;
};

static RenderContetxt_Data* S_Data;

static void ParseShader(const std::string& code, RenderContext_Shader* shader)
{
	s32 index = 0;
	while ((index = code.find("uniform", index)) != -1)
	{
		index += 7;
		index = code.find_first_not_of(" \n\t", index);
		s32 spaceIndex = code.find(' ', index);
		std::string uniformType = code.substr(index, spaceIndex - index);
		index = spaceIndex;
		index = code.find_first_not_of(" \n\t", index);
		s32 semicolonIndex = code.find(';', index);
		std::string uniformName = code.substr(index, semicolonIndex - index);

		u32 location = glGetUniformLocation(shader->Program, uniformName.c_str());
		shader->UniformStringMap[uniformName] = location;
	}
}

static void LoadShader(const std::string& filePath, RenderContext_Shader* shader)
{
	//Read shader from file		
	std::ifstream shaderFile(filePath);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	std::string shaderCode = shaderStream.str();

	s32 vertexHeaderStart = shaderCode.find("#vertex");
	s32 fragmentHeaderStart = shaderCode.find("#fragment", vertexHeaderStart);

	if (vertexHeaderStart == -1 || fragmentHeaderStart == -1)
	{
		std::cerr << "Could not find vertex or fragment shader \n\tForgot #vertex or #fragment" << std::endl;
		return;
	}

	std::string vertexCode = shaderCode.substr(vertexHeaderStart + 8, fragmentHeaderStart - 8);
	std::string fragmentCode = shaderCode.substr(fragmentHeaderStart + 10);

	const char* vertexCode_cstr = vertexCode.c_str();
	const char* fragmentCode_cstr = fragmentCode.c_str();

	char infoLog[512];

	shader->VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->VertexShader, 1, &vertexCode_cstr, 0);
	glCompileShader(shader->VertexShader);
	GLint success = 0;
	glGetShaderiv(shader->VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader->VertexShader, 512, NULL, infoLog);
		std::cerr << "Could not compile vertex shader :\n\t" << infoLog << std::endl;
		return;
	}

	shader->FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->FragmentShader, 1, &fragmentCode_cstr, 0);
	glCompileShader(shader->FragmentShader);
	glGetShaderiv(shader->FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader->FragmentShader, 512, NULL, infoLog);
		std::cerr << "Could not compile fragment shader :\n\t" << infoLog << std::endl;
		return;
	}

	shader->Program = glCreateProgram();
	glAttachShader(shader->Program, shader->VertexShader);
	glAttachShader(shader->Program, shader->FragmentShader);
	glLinkProgram(shader->Program);
	glGetProgramiv(shader->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader->Program, 512, NULL, infoLog);
		std::cout << "Could not link shader program" << infoLog << std::endl;
		return;
	}

	ParseShader(shaderCode, shader);
}

static void LoadAllShaders()
{
	RenderContext_Shader shader;

	LoadShader("Res/Shaders/Test.glsl", &S_Data->Shaders[Shader_Test]);
}

void RenderContext::Init()
{
	if (glewInit() != GLEW_OK)
	{
		//TODO
	}

	glEnable(GL_DEPTH_TEST);

	S_Data = new RenderContetxt_Data();
	LoadAllShaders();
	std::cout << "Loaded all shaders" << std::endl;
}

void RenderContext::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::BindShader(Shader shader)
{
	S_Data->BoundShader = shader;
	glUseProgram(S_Data->Shaders[shader].Program);
}

void RenderContext::SetUniform(const std::string& name, r32 value)
{
	RenderContext_Shader* shader = &S_Data->Shaders[S_Data->BoundShader];
	const auto&& it = shader->UniformStringMap.find(name);
	if (it == shader->UniformStringMap.end())
	{
		std::cerr << "Could not find uniform '" << name << "'" << std::endl;
		return;
	}
	glUniform1f(it->second, value);
}

void RenderContext::SetUniform(const std::string& name, const v2& value)
{
	RenderContext_Shader* shader = &S_Data->Shaders[S_Data->BoundShader];
	const auto&& it = shader->UniformStringMap.find(name);
	if (it == shader->UniformStringMap.end())
	{
		std::cerr << "Could not find uniform '" << name << "'" << std::endl;
		return;
	}
	glUniform2f(it->second, value.x, value.y);
}

void RenderContext::SetUniform(const std::string& name, const v3& value)
{
	RenderContext_Shader* shader = &S_Data->Shaders[S_Data->BoundShader];
	const auto&& it = shader->UniformStringMap.find(name);
	if (it == shader->UniformStringMap.end())
	{
		std::cerr << "Could not find uniform '" << name << "'" << std::endl;
		return;
	}
	glUniform3f(it->second, value.x, value.y, value.z);
}

void RenderContext::SetUniform(const std::string& name, const m4& value)
{
	RenderContext_Shader* shader = &S_Data->Shaders[S_Data->BoundShader];
	const auto&& it = shader->UniformStringMap.find(name);
	if (it == shader->UniformStringMap.end())
	{
		std::cerr << "Could not find uniform '" << name << "'" << std::endl;
		return;
	}
	glUniformMatrix4fv(it->second, 1, false, value[0]);
}

Renderable RenderContext::CreateRenderable3D(const Model& model)
{
	Renderable3D_Data renderable;
	renderable.count = model.Indices.size();
	glGenVertexArrays(1, &renderable.vao);
	glGenBuffers(1, &renderable.vbo);
	glGenBuffers(1, &renderable.ibo);

	glBindVertexArray(renderable.vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderable.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.ibo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model.Vertices.size(), &model.Vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * model.Indices.size(), &model.Indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Vertex::Pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Vertex::Color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	S_Data->Renderables.push_back(renderable);
	return S_Data->Renderables.size() - 1;
}

void RenderContext::Render(Renderable renderable)
{
	const Renderable3D_Data& data = S_Data->Renderables[renderable];

	glBindVertexArray(data.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibo);

	glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
}
