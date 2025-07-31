#include "pch.h"
#include "LoaderModel.h"
#include <AssImp/Importer.hpp>
#include <AssImp/scene.h>
#include <AssImp/postprocess.h>

LoaderModel::LoaderModel()
{
}

bool LoaderModel::DoTheImportThing(const std::string& aFile)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aFile, 
		aiProcess_CalcTangentSpace | 
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices | 
		aiProcess_SortByPType);
	if (!scene)
	{
		// importer.GetErrorString();
		return false;
	}
	return true;
}
