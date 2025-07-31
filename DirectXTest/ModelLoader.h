#pragma once
#include <string>

#include "Model.h"

class ModelLoader
{
public:
	ModelLoader() = default;
	~ModelLoader() = default;

	CModel* GetModel(std::string& aFilePath);
private:
};

