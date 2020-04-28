#include "Model.h"
#include <fstream>
#include <iostream>

static b32 VerifyHeader(FILE* file)
{
	char format[6];
	fread(format, 1, 5, file);
	format[5] = 0;

	if (strcmp(format, "pfmdl") != 0)
	{
		std::cerr << "This is not a pfmdl file" << std::endl;
		return false;
	}

	return true;
}

void ModelLoader::LoadPFMDL(const std::string& path, Model* model)
{
	FILE* file = fopen(path.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Could not find model file" << std::endl;
		return;
	}

	if (!VerifyHeader(file))
		return;

	u32 numVertices = 0;
	u32 numIndices = 0;
	fread(&numVertices, sizeof(u32), 1, file);
	fread(&numIndices, sizeof(u32), 1, file);

	model->Vertices.resize(numVertices);
	model->Indices.resize(numIndices);

	fread(&model->Vertices[0], sizeof(Vertex), numVertices, file);
	fread(&model->Indices[0], sizeof(u32), numIndices, file);

	fclose(file);
}
