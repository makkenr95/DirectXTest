#pragma once
#include "GraphicsEngine.h"

class CLightFactory;
class CCameraFactory;
class CModelFactory;

namespace CommonUtilities
{
	class Timer;
}

class Engine
{
public:
	~Engine();
	int Init(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR lpCmdLine,
		_In_ int nCmdShow);
	void Update();
	void Exit();
	bool WindowHandler();
	static Engine* GetInstance();

	CGraphicsEngine* GetGraphicsEngine() const;
	CommonUtilities::Timer* GetTimer() const;
	CCamera* GetCamera() const;
	CScene* GetScene() const;
	CModelFactory* GetModelFactory() const;
	CCameraFactory* GetCameraFactory() const;
	CLightFactory* GetLightFactory() const;
private:
	Engine();
	inline static Engine* ourInstance = nullptr;

	CGraphicsEngine* myGraphicsEngine;
	CommonUtilities::Timer* myTimer = nullptr;
	CCamera* myCamera = nullptr;
	CScene* myScene = nullptr;
	CModelFactory* myModelFactory = nullptr;
	CCameraFactory* myCameraFactory = nullptr;
	CLightFactory* myLightFactory = nullptr;


	//TEMP
	CModelInstance* model2 = nullptr;
	CModelInstance* model3 = nullptr;
	CModelInstance* head0 = nullptr;

};

