#pragma once
#include <d3d11.h>
#include <string>
#include "Model.h"

class CModelFactory
{
public:
	~CModelFactory();
	std::vector<CModel*> GetModel(std::string aFilePath);
	void SetDevice(ID3D11Device* aDevice);
	static CModelFactory* GetInstance();
	CModel* LoadModel(std::string aFilePath);
	CModel* GetCube() const;
private:
	CModelFactory();
	static CModelFactory* ourInstance;
	ID3D11Device* myDevice;
};

