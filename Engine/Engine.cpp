#include "pch.h"
#include "Engine.h"
#include "CameraFactory.h"
#include "LightFactory.h"
#include "MainSingleton.h"
#include "ModelFactory.h"
#include "Timer.h"
#include "Scene.h"

Engine::Engine()
{
	ourInstance = this;
}

Engine::~Engine()
{
	// delete myScene;
	// myScene = nullptr;
	// delete myTimer;
	// myTimer = nullptr;
	// delete myCamera;
	// myCamera = nullptr;
	// delete model2;
	// model2 = nullptr;
	// delete model3;
	// model3 = nullptr;
}

int Engine::Init(_In_ HINSTANCE /*hInstance*/,
                 _In_opt_ HINSTANCE /*hPrevInstance*/,
                 _In_ LPWSTR /*lpCmdLine*/,
                 _In_ int /*nCmdShow*/)
{
	MainSingleton* mainSingleton = new MainSingleton;
	mainSingleton->Create();
	myGraphicsEngine = new CGraphicsEngine;
	myTimer = mainSingleton->GetTimer();

	if (WindowHandler() == false)
	{
		return EXIT_FAILURE;
	}

	myModelFactory = CModelFactory::GetInstance();
	myCameraFactory = CCameraFactory::GetInstance();
	myLightFactory = CLightFactory::GetInstance();

	CModelFactory* modelFactory = CModelFactory::GetInstance();
	modelFactory->SetDevice(myGraphicsEngine->GetFramework().GetDevice());
	
	myScene = CScene::GetInstance();
	return EXIT_SUCCESS;
}

void Engine::Update()
{
	myTimer->Update();
	myGraphicsEngine->BeginFrame();
	myGraphicsEngine->RenderFrame();
	myGraphicsEngine->EndFrame();
}

void Engine::Exit()
{
	MainSingleton::GetInstance()->Destory();
}

bool Engine::WindowHandler()
{
	CWindowHandler::SWindowData windowData;
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1600;
	windowData.myHeight = 900;
	return myGraphicsEngine->Init(windowData);
}

Engine* Engine::GetInstance()
{
	if (!ourInstance)
	{
		ourInstance = new Engine();
	}
	return ourInstance;
}

CGraphicsEngine* Engine::GetGraphicsEngine() const
{
	return myGraphicsEngine;
}

CommonUtilities::Timer* Engine::GetTimer() const
{
	return myTimer;
}

CCamera* Engine::GetCamera() const
{
	return myCamera;
}

CScene* Engine::GetScene() const
{
	return myScene;
}

CModelFactory* Engine::GetModelFactory() const
{
	return myModelFactory;
}

CCameraFactory* Engine::GetCameraFactory() const
{
	return myCameraFactory;
}

CLightFactory* Engine::GetLightFactory() const
{
	return myLightFactory;
}
