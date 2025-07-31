 // DirectXTest.cpp : Defines the entry point for the application.
 //
#include "pch.h"
 #include "DirectXTest.h"

 #include "CameraFactory.h"
 #include "CPointLight.h"
 #include "CSpotLight.h"
 #include "GraphicsEngine.h"
 #include "LightFactory.h"
 #include "MainSingleton.h"
 #include "ModelFactory.h"
 #include "ModelInstance.h"
 #include "Scene.h"
 #include "Timer.h"
 #include "EnvironmentLight.h"

 int APIENTRY wWinMain(_In_ HINSTANCE /*hInstance*/,
 					_In_opt_ HINSTANCE /*hPrevInstance*/,
 					_In_ LPWSTR /*lpCmdLine*/,
 					_In_ int /*nCmdShow*/)
 {
 	MainSingleton* mainSingleton = new MainSingleton;
 	mainSingleton->Create();

 	CU::Timer* timer = mainSingleton->GetTimer();

 	CWindowHandler::SWindowData windowData;
 	windowData.myX = 100;
 	windowData.myY = 100;
 	windowData.myWidth = 1600;
 	windowData.myHeight = 900;

 	CGraphicsEngine graphicsEngine;
 	bool shouldRun = graphicsEngine.Init(windowData);

 	CModelFactory* modelFactory = CModelFactory::GetInstance();
 	modelFactory->SetDevice(graphicsEngine.GetFramework().GetDevice());
 	std::vector<CModel*> chest = modelFactory->GetModel("Chest/Particle_Chest.fbx");
 	std::vector<CModel*> head = modelFactory->GetModel("Head/mHead.fbx");
 	// if (!models)
 	// {
 	// 	models = modelFactory->GetCube();
 	// 	if (!models)
 	// 	{
 	// 		return EXIT_FAILURE;
 	// 	}
 	// }
 	CModelInstance* modelInstance = new CModelInstance();
 	modelInstance->Init(chest);
 	modelInstance->SetPosition({-10, -10, 500});

 	CModelInstance* model2 = new CModelInstance;
 	model2->Init(chest);
 	model2->SetPosition({-15, -200, 500});

 	CModelInstance* model3 = new CModelInstance;
 	model3->Init(chest);
 	model3->SetPosition({500, -200, 500});

 	CModelInstance* head0 = new CModelInstance;
 	head0->Init(head);
 	head0->SetPosition({0, 0, 0});

 	CScene* scene = CScene::GetInstance();
 	scene->AddInstance(modelInstance);
 	scene->AddInstance(model2);
 	scene->AddInstance(model3);
 	scene->AddInstance(head0);

 	CCameraFactory* cameraFactory = CCameraFactory::GetInstance();
 	CCamera* camera = cameraFactory->CreateCamera(90.0f);
 	camera->SetTransform({0, 0, -5.0f}, {0, 0, 0});
 	scene->AddInstance(camera);
 	scene->SetMainCamera(camera);

 	CLightFactory* lightFactory = CLightFactory::GetInstance();
 	lightFactory->Init(graphicsEngine.GetFramework().GetDevice());
 	CEnvironmentLight* environmentLight = lightFactory->CreateEnviornmentLight(L"cube_1024_preblurred_angle3_Skansen3.dds");
 	environmentLight->SetDirection({1, 0, 0, 0});
 	//environmentLight->SetColor({.8f, .8f, .8f, 1.f});
 	environmentLight->SetColor({0.f, 0.f, 0.f, 0.f});
 	scene->AddInstance(environmentLight);

 	CPointLight* pointLight = lightFactory->CreatePointLight();
 	pointLight->SetPosition({3,0,0});
 	pointLight->SetColor({.0f, 1.f, .0f, 1000000.f});
 	pointLight->SetRange(500.0f);
 	scene->AddInstance(pointLight);

 	CPointLight* point2Light = lightFactory->CreatePointLight();
 	point2Light->SetPosition({-1,0,500});
 	point2Light->SetColor({1.f, .0f, .0f, 1000000.f});
 	point2Light->SetRange(500.0f);
 	scene->AddInstance(point2Light);

 	CPointLight* point3Light = lightFactory->CreatePointLight();
 	point3Light->SetPosition({0,500,500});
 	point3Light->SetColor({.0f, .0f, 1.f, 1000000.f});
 	point3Light->SetRange(500.0f);
 	scene->AddInstance(point3Light);

 	CSpotLight* spotLight = lightFactory->CreateSpotLight();
 	spotLight->SetPosition({ 0.0f, 0.0f, 0.0f, 0 });
 	spotLight->SetColor({ 1.0f, 0.0f, 1.0f, 50000.f });
 	spotLight->SetDirection({ 0.0f, 0.0f, 1.0f, 0 });
 	spotLight->SetRange(2500.0f);
 	spotLight->SetInnerAngle(0.0f);
 	spotLight->SetOuterAngle(0.3f);
 	scene->AddInstance(spotLight);

 	MSG windowMessage = {nullptr};

 	// Main message loop:
 	while (shouldRun)
 	{
 		while (PeekMessage(&windowMessage, nullptr, 0, 0, PM_REMOVE))
 		{
 			TranslateMessage(&windowMessage);
 			DispatchMessage(&windowMessage);
 			if (windowMessage.message == WM_QUIT)
 			{
 				shouldRun = false;
 			}
 		}
 		timer->Update();
 		// cubeInstance->Rotate({.001f,.002f,.003f});
 		camera->Update(timer->GetDeltaTime());
 		model2->Rotate({0, -.01f, 0});
 		model3->Rotate({0, .01f, 0});
 		graphicsEngine.BeginFrame();
 		graphicsEngine.RenderFrame();
 		graphicsEngine.EndFrame();
 	}
 	MainSingleton::GetInstance()->Destory();
 	return 0;
 }
