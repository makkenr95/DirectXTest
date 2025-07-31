#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "ModelFactory.h"
#include "ModelInstance.h"
#include "Scene.h"
#include "CameraFactory.h"
#include "CPointLight.h"
#include "CSpotLight.h"
#include "EnvironmentLight.h"
#include "LightFactory.h"


void Game::Init()
{
	std::vector<CModel*> chest = Engine::GetInstance()->GetModelFactory()->GetModel("Chest/Particle_Chest.fbx");
	std::vector<CModel*> head = Engine::GetInstance()->GetModelFactory()->GetModel("Head/mHead.fbx");

	myScene = Engine::GetInstance()->GetScene();

	CModelInstance* modelInstance = new CModelInstance();
	modelInstance->Init(chest);
	modelInstance->SetPosition({ -10, -10, 500 });

	model2 = new CModelInstance;
	model2->Init(chest);
	model2->SetPosition({ -15, -200, 500 });

	model3 = new CModelInstance;
	model3->Init(chest);
	model3->SetPosition({ 500, -200, 500 });

	head0 = new CModelInstance;
	head0->Init(head);
	head0->SetPosition({ 0,	0, 2000 });

	myScene = Engine::GetInstance()->GetScene();
	myScene->AddInstance(modelInstance);
	myScene->AddInstance(model2);
	myScene->AddInstance(model3);
	myScene->AddInstance(head0);

	CCameraFactory* cameraFactory = Engine::GetInstance()->GetCameraFactory()->GetInstance();
	myCamera = cameraFactory->CreateCamera(90.0f);
	myCamera->SetTransform({ 0, 0, -5.0f }, { 0, 0, 0 });
	myScene->AddInstance(myCamera);
	myScene->SetMainCamera(myCamera);

	CLightFactory* lightFactory = Engine::GetInstance()->GetLightFactory();
	CGraphicsEngine* graphicsEngine = Engine::GetInstance()->GetGraphicsEngine();
	lightFactory->Init(graphicsEngine->GetFramework().GetDevice());
	CEnvironmentLight* environmentLight = lightFactory->CreateEnviornmentLight(L"cube_1024_preblurred_angle3_Skansen3.dds");
	environmentLight->SetDirection({ 1, 0, 0, 0 });
	environmentLight->SetColor({.8f, .8f, .8f, 1.f});
	myScene->AddInstance(environmentLight);

	CPointLight* pointLight = lightFactory->CreatePointLight();
	pointLight->SetPosition({ 3,0,0 });
	pointLight->SetColor({ .0f, 1.f, .0f, 1000000.f });
	pointLight->SetRange(500.0f);
	myScene->AddInstance(pointLight);
	
	CPointLight* point2Light = lightFactory->CreatePointLight();
	point2Light->SetPosition({ -1,0,500 });
	point2Light->SetColor({ 1.f, .0f, .0f, 1000000.f });
	point2Light->SetRange(500.0f);
	myScene->AddInstance(point2Light);
	
	CPointLight* point3Light = lightFactory->CreatePointLight();
	point3Light->SetPosition({ 0,500,500 });
	point3Light->SetColor({ .0f, .0f, 1.f, 1000000.f });
	point3Light->SetRange(500.0f);
	myScene->AddInstance(point3Light);
	
	CPointLight* point4Light = lightFactory->CreatePointLight();
	point4Light->SetPosition({ 0,0, 1800 });
	point4Light->SetColor({ .0f, .0f, .0f, 1000000.f });
	point4Light->SetRange(300.0f);
	myScene->AddInstance(point4Light);
	
	CSpotLight* spotLight = lightFactory->CreateSpotLight();
	spotLight->SetPosition({ 0.0f, 0.0f, 0.0f, 0 });
	spotLight->SetColor({ 1.0f, 0.0f, 1.0f, 50000.f });
	spotLight->SetDirection({ 0.0f, 0.0f, 1.0f, 0 });
	spotLight->SetRange(2500.0f);
	spotLight->SetInnerAngle(0.0f);
	spotLight->SetOuterAngle(0.3f);
	myScene->AddInstance(spotLight);
}

void Game::Update(float aDeltaTime)
{
	myCamera->Update(aDeltaTime);
	model2->Rotate({ 0, -.01f, 0 });
	model3->Rotate({ 0, .01f, 0 });
}
