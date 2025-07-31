#include "pch.h"
#include "RenderManager.h"
#include "DirectX11Framework.h"
#include "FullscreenTextureFactory.h"
#include <memory>
#include "EnvironmentLight.h"
#include "Camera.h"

RenderManager::RenderManager(CScene* aScene, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, CDirectX11Framework* aFramework) : myScene(aScene), myDevice(aDevice), myContext(aContext), myFramework(aFramework)
{
	myBackBuffer = FullscreenTexture(myContext);
	myIntermediateDepth = FullscreenTexture(myContext);

	myHalfsizeTexture = FullscreenTexture(myContext);
	myLuminanceTexture = FullscreenTexture(myContext);
	myQuartersizeTexture = FullscreenTexture(myContext);
	myBlurTexture1 = FullscreenTexture(myContext);
	myBlurTexture2 = FullscreenTexture(myContext);
}

RenderManager::~RenderManager()
{
}

bool RenderManager::Init()
{
	if (!myFowardRenderer.Init(myFramework))
		return false;
	if (!myFullscreenRender.Init(myContext, myDevice))
		return false;

	ID3D11Resource* backBufferResource = nullptr;
	myFramework->GetBackBuffer()->GetResource(&backBufferResource);
	ID3D11Texture2D* backBufferTexture = reinterpret_cast<ID3D11Texture2D*>(backBufferResource);
	if (!backBufferTexture)
		return false;
	FullscreenTextureFactory factory = FullscreenTextureFactory(myContext, myDevice);
	myBackBuffer = factory.CreateTexture(backBufferTexture);
	myIntermediateDepth = factory.CreateDepth({ 1600, 900 }, DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT);
	myIntermediateTexture = factory.CreateTexture({ 1600, 900 }, DXGI_FORMAT_R8G8B8A8_UNORM);

	myHalfsizeTexture = factory.CreateTexture({800,450}, DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = factory.CreateTexture({1600,900}, DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuartersizeTexture = factory.CreateTexture({400,225}, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = factory.CreateTexture({1600,900}, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = factory.CreateTexture({1600,900}, DXGI_FORMAT_R8G8B8A8_UNORM);

	return true;
}

void RenderManager::Render()
{
	myBackBuffer.ClearTexture();
	myIntermediateTexture.ClearTexture();
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);
	//
	// const std::shared_ptr<CCamera> renderCamera = std::make_shared<CCamera>(*myScene->GetMainCamera());
	// const std::shared_ptr<CEnvironmentLight> directionalLight = std::make_shared<CEnvironmentLight>(*myScene->GetEnvironmentLight());
	// //const std::shared_ptr<CCamera> renderCamera = std::make_shared<CCamera>(*myScene.GetMainCamera());
	//
	// std::vector<CModelInstance*> models = myScene->CullModels(renderCamera.get());
	// //for (auto model : models)
	// //{
	//	
	//}

	CEnvironmentLight* environmentLight = myScene->GetEnvironmentLight();
	CCamera* mainCamera = myScene->GetMainCamera();
	std::vector<CModelInstance*> modelsToRender = CScene::GetInstance()->CullModels(mainCamera);
	// std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	std::vector<CPointLight*> pointLights;
	for (int i = 0; i < modelsToRender.size(); ++i)
	{
		pointLights = myScene->CullLights(modelsToRender[i]);
	}
	pointLights.clear();
	std::vector<CSpotLight*> spotLights;
	for (int i = 0; i < modelsToRender.size(); ++i)
	{
		spotLights = myScene->CullSpotLights(modelsToRender[i]);
	}
	spotLights.clear();
	myFowardRenderer.Render(mainCamera, pointLights, spotLights, environmentLight, modelsToRender);


	//myFowardRenderer.Render(renderCamera.get(), directionalLight.get(), models);

	myLuminanceTexture.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::LUMINACE);

	myHalfsizeTexture.SetAsActiveTarget();
	myLuminanceTexture.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::COPY);
	
	myQuartersizeTexture.SetAsActiveTarget();
	myHalfsizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::COPY);
	
	myBlurTexture1.SetAsActiveTarget();
	myQuartersizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::COPY);

	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::GAUSSIANVERTICAL);

	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::GAUSSIANVERTICAL);

	myQuartersizeTexture.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::COPY);

	myHalfsizeTexture.SetAsActiveTarget();
	myQuartersizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRender.Render(FullscreenRender::Shader::COPY);

	myBackBuffer.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myHalfsizeTexture.SetAsResourceOnSlot(1);
	myFullscreenRender.Render(FullscreenRender::Shader::BLOOM);
}
