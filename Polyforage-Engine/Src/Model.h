#pragma once
#include <vector>
#include <string>
#include "Math.h"

struct Vertex
{
	Vertex() :
		Pos(0.0f, 0.0f, 0.0f),
		Color(1.0f, 1.0f, 1.0f)
	{}

	Vertex(const v3& pos, const v3& color) :
		Pos(pos), Color(color)
	{}

	v3 Pos;
	v3 Color;
};

struct Model
{
	std::vector<Vertex> Vertices;
	std::vector<u32> Indices;
};

class ModelLoader
{
public:
	static void LoadPFMDL(const std::string& path, Model* model);
};