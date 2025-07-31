#pragma once
#include "Scene.h"
#include "FowardRenderer.h"
#include "FullscreenRender.h"
#include "FullscreenTexture.h"

class RenderManager
{
public:
	RenderManager(CScene* aScene, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, CDirectX11Framework* aFramework);
	~RenderManager();
	bool Init();
	void Render();
private:
	CScene* myScene;
	CFowardRenderer myFowardRenderer;
	FullscreenRender myFullscreenRender;

	FullscreenTexture myBackBuffer;
	FullscreenTexture myIntermediateDepth;
	FullscreenTexture myIntermediateTexture;

	FullscreenTexture myHalfsizeTexture;
	FullscreenTexture myLuminanceTexture;
	FullscreenTexture myQuartersizeTexture;
	FullscreenTexture myBlurTexture1;
	FullscreenTexture myBlurTexture2;

	ID3D11Device* myDevice = nullptr;
	ID3D11DeviceContext* myContext = nullptr;
	CDirectX11Framework* myFramework = nullptr;
};

