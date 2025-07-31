#pragma once
#include "Camera.h"

class CCameraFactory
{
public:
	~CCameraFactory();
	static CCameraFactory* GetInstance();
	CCamera* CreateCamera(float aFoV);
private:
	CCameraFactory();
	static CCameraFactory* ourInstance;
};

