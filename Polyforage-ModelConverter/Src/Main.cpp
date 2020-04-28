#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <iostream>

typedef unsigned int u32;
typedef signed int s32;
typedef u32 b32;

static const aiScene* Scene;

struct vertex
{
	aiVector3D Pos;
	aiVector3D Color;
};

struct model
{
	vertex* Vertices;
	u32 NumVertices;

	u32* Indices;
	u32 NumIndices;
};

u32 aiStringLength(const aiString& string)
{
	u32 length = 0;
	while (string.data[length] != 0)
		length++;
	return length;
}

void CreateErrorFile(const char* Message)
{
	FILE* File = fopen("Error.txt", "w");
	fputs(Message, File);
	fclose(File);
	std::exit(1);
}

void WriteHeader(FILE* File)
{
	char Header[5];
	strcpy(Header, "pfmdl");
	fwrite(Header, 1, 5, File);
}

void WriteMetadata(FILE* File, model* Model)
{
	u32 Metadata[2];
	Metadata[0] = Model->NumVertices;
	Metadata[1] = Model->NumIndices;
	fwrite(Metadata, sizeof(u32), 2, File);
}

void WriteModelToFile(const char* OriginalPath, model* Model)
{
	//		Res/Models/Place/cube.fbx	->	Res/Models/Place/cube.pfmdl

	char NewPath[512];
	u32 Index = 0;
	while (OriginalPath[Index] != '.')
	{
		NewPath[Index] = OriginalPath[Index];
		Index++;
	}

	strcpy(&NewPath[Index], ".pfmdl");
	FILE* File = fopen(NewPath, "wb");

	WriteHeader(File);
	WriteMetadata(File, Model);
	
	fwrite(Model->Vertices, sizeof(vertex), Model->NumVertices, File);
	fwrite(Model->Indices, sizeof(u32), Model->NumIndices, File);

	fclose(File);
}

void LoadModel(model* Model)
{
	for (u32 i = 0; i < Scene->mNumMeshes; i++)
	{
		Model->NumVertices += Scene->mMeshes[i]->mNumVertices;
		Model->NumIndices += Scene->mMeshes[i]->mNumFaces * 3;
	}

	Model->Vertices = (vertex*)malloc(sizeof(vertex) * Model->NumVertices);
	Model->Indices = (u32*)malloc(sizeof(u32) * Model->NumIndices);

	u32 VertexOffset = 0;
	u32 IndexOffset = 0;
	u32 LastIndex = 0;

	for (u32 i = 0; i < Scene->mNumMeshes; i++)
	{
		const aiMesh* Mesh = Scene->mMeshes[i];
		for (u32 j = 0; j < Mesh->mNumVertices; j++)
		{
			const aiVector3D& Pos = Mesh->mVertices[j];
			const aiColor4D& Color = Mesh->HasVertexColors(0) ? Mesh->mColors[j][0] : aiColor4D(1.0f, 1.0f, 1.0f, 1.0);

			vertex Vertex;
			Vertex.Pos = Pos;
			Vertex.Color = aiVector3D(Color.r, Color.g, Color.b);

			Model->Vertices[j + VertexOffset] = Vertex;
		}

		u32 index = 0;
		for (u32 j = 0; j < Mesh->mNumFaces; j++)
		{
			Model->Indices[index++ + IndexOffset] = Mesh->mFaces[j].mIndices[0] + LastIndex;
			Model->Indices[index++ + IndexOffset] = Mesh->mFaces[j].mIndices[1] + LastIndex;
			Model->Indices[index++ + IndexOffset] = Mesh->mFaces[j].mIndices[2] + LastIndex;
		}

		VertexOffset += Mesh->mNumVertices;
		IndexOffset += Mesh->mNumFaces * 3;
		LastIndex += Mesh->mNumVertices;
	}

	
}


int main(int argc, char** argv)
{
	const char* FilePath = argv[1];
	Assimp::Importer Importer;
	Scene = Importer.ReadFile(FilePath, aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials |
		aiProcess_GenSmoothNormals);

	if (!Scene)
		CreateErrorFile(Importer.GetErrorString());

	model Model;
	Model.NumVertices = 0;
	Model.NumIndices = 0;

	LoadModel(&Model);
	WriteModelToFile(FilePath, &Model);
}