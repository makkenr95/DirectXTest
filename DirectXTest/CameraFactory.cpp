#include "pch.h"
#include "CameraFactory.h"

CCameraFactory* CCameraFactory::ourInstance = nullptr;

CCameraFactory::~CCameraFactory()
{
	ourInstance = nullptr;
}

CCamera* CCameraFactory::CreateCamera(float aFoV)
{
	CCamera* camera = new CCamera();
	camera->Init(aFoV, { 1600,900 });
	return camera;
}

CCameraFactory::CCameraFactory()
{
	ourInstance = this;
}

CCameraFactory* CCameraFactory::GetInstance()
{
	if (!ourInstance)
	{
		ourInstance = new CCameraFactory();
	}
	return ourInstance;
}
