#include "pch.h"
#include "ModelLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

CModel* ModelLoader::GetModel(std::string& aFilePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aFilePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	if (!scene)
	{
		// importer.GetErrorString();
		return nullptr;
	}

	aiMesh** meshses = scene->mMeshes;

	(*meshses)->mVertices;

	return nullptr;
}
