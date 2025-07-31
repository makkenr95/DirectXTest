#include "pch.h"
#include "Scene.h"
#include "Camera.h"

CScene* CScene::ourInstance = nullptr;

CScene::CScene()
{
	ourInstance = this;
}

CScene::~CScene()
{
	ourInstance = nullptr;
	delete myMainCamera;
	myMainCamera = nullptr;
}

CCamera* CScene::GetMainCamera()
{
	return myMainCamera;
}

std::vector<CPointLight*> CScene::CullLights(CModelInstance* /*anInstance*/) const
{
	return myPointLights;
}

std::vector<CSpotLight*> CScene::CullSpotLights(CModelInstance* /*anInstance*/) const
{
	return mySpotLights;
}

CScene* CScene::GetInstance()
{
	if (!ourInstance)
	{
		ourInstance = new CScene();
	}
	return ourInstance;
}

bool CScene::Init()
{
	return true;
}

bool CScene::AddInstance(CModelInstance* aModelInstance)
{
	myModels.push_back(aModelInstance);
	return true;
}

bool CScene::AddInstance(CCamera* aCameraInstance)
{
	myCameras.emplace_back(aCameraInstance);
	return true;
}

std::vector<CModelInstance*> CScene::CullModels(CCamera* /*aCamera*/) const
{
	return myModels;
}

void CScene::SetMainCamera(CCamera* const aMainCamera)
{
	myMainCamera = aMainCamera;
}

CEnvironmentLight* CScene::GetEnvironmentLight() const
{
	return myEnvironmentLight;
}

void CScene::AddInstance(CEnvironmentLight* const anEnvironmentLight)
{
	myEnvironmentLight = anEnvironmentLight;
}

bool CScene::AddInstance(CPointLight* aPointLight)
{
	// myPointLights.fill(aPointLight); // TODO FIXA DETTA!!!!!
	myPointLights.push_back(aPointLight); // TODO FIXA DETTA!!!!! kolla antal ljus
	return true;
}

bool CScene::AddInstance(CSpotLight* aSpotLight)
{
	mySpotLights.push_back(aSpotLight);
	return true;
}
