#include "pch.h"
#include "GraphicsEngine.h"
#include <array>
#include "Scene.h"
#include "CPointLight.h"
#include "RenderManager.h"

CGraphicsEngine::CGraphicsEngine()
{
	myScene = CScene::GetInstance();
	myWindowHandler = CWindowHandler();
}

CGraphicsEngine::~CGraphicsEngine()
{
	delete myScene;
	myScene = nullptr;
}

bool CGraphicsEngine::Init(CWindowHandler::SWindowData aWindowData)
{
	if (!myWindowHandler.Init(aWindowData))
	{
		return false;
	}
	if (!myFramework.Init(&myWindowHandler))
	{
		return false;
	}
	if(!myFowardRenderer.Init(&myFramework))
	{
		return false;
	}

	myRenderManager = new RenderManager(myScene, myFramework.GetDevice(), myFramework.GetContext(), &myFramework);
	if (!myRenderManager->Init())
	{
		return false;
	}
	return true;
}

void CGraphicsEngine::BeginFrame()
{
	std::array clearColor = { .25f,.5f, 1.f,1.f };
	myFramework.BeginFrame(clearColor);
}

void CGraphicsEngine::RenderFrame()
{
	// CEnvironmentLight* environmentLight = myScene->GetEnvironmentLight();
	// CCamera* mainCamera = myScene->GetMainCamera();
	// std::vector<CModelInstance*> modelsToRender = CScene::GetInstance()->CullModels(mainCamera);
	// // std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	// std::vector<CPointLight*> pointLights;
	// for (int i = 0; i < modelsToRender.size(); ++i)
	// {
	// 	pointLights = myScene->CullLights(modelsToRender[i]);
	// }
	//
	// std::vector<CSpotLight*> spotLights;
	// for (int i = 0; i < modelsToRender.size(); ++i)
	// {
	// 	spotLights = myScene->CullSpotLights(modelsToRender[i]);
	// }
	// myFowardRenderer.Render(mainCamera, pointLights, spotLights, environmentLight, modelsToRender);
	myRenderManager->Render();
}

void CGraphicsEngine::EndFrame()
{
	myFramework.EndFrame();
}

CDirectX11Framework& CGraphicsEngine::GetFramework()
{
	return myFramework;
}
