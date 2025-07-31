#pragma once
#include <d3d11.h>
#include <string>

class CSpotLight;
class CPointLight;
class CEnvironmentLight;

class CLightFactory
{
public:
	void Init(ID3D11Device* aDevice);
	~CLightFactory();
	CEnvironmentLight* CreateEnviornmentLight(std::wstring aPath);
	CPointLight* CreatePointLight();
	CSpotLight* CreateSpotLight();
	static CLightFactory* GetInstance();
private:
	CLightFactory();
	static CLightFactory* ourInstance;
	ID3D11Device* myDevice = nullptr;
};

