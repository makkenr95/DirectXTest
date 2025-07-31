#pragma once
#include "WindowHandler.h"
#include "DirectX11Framework.h"
#include "FowardRenderer.h"

class RenderManager;
class CScene;

class CGraphicsEngine
{
public:
	 CGraphicsEngine();
	 ~CGraphicsEngine();
	bool Init(CWindowHandler::SWindowData aWindowData);
	void BeginFrame();
	void RenderFrame();
	void EndFrame();
	CDirectX11Framework& GetFramework();
private:
	CWindowHandler myWindowHandler;
	CDirectX11Framework myFramework;
	CScene* myScene;
	CFowardRenderer myFowardRenderer;
	RenderManager* myRenderManager = nullptr;

};


