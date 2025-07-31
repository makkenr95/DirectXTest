#include "pch.h"
#include "LightFactory.h"
#include <cassert>

#include "CPointLight.h"
#include "CSpotLight.h"
#include "DDSTextureLoader.h"
#include "EnvironmentLight.h"

CLightFactory* CLightFactory::ourInstance = nullptr;

void CLightFactory::Init(ID3D11Device* aDevice)
{
	myDevice = aDevice;
}

CLightFactory::~CLightFactory()
{
	ourInstance = nullptr;
}

CEnvironmentLight* CLightFactory::CreateEnviornmentLight(std::wstring aPath)
{
	HRESULT result;
	ID3D11ShaderResourceView* envShaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, aPath.c_str(), nullptr, &envShaderResourceView);
	if (FAILED(result))
	{
		assert(false && "Fel med att läsa in cube map");
		return nullptr;
	}
	return new CEnvironmentLight(envShaderResourceView);
}

CPointLight* CLightFactory::CreatePointLight()
{
	return new CPointLight;
}

CSpotLight* CLightFactory::CreateSpotLight()
{
	return new CSpotLight;
}

CLightFactory* CLightFactory::GetInstance()
{
	if (!ourInstance)
	{
		ourInstance = new CLightFactory;
	}
	return ourInstance;
}

CLightFactory::CLightFactory()
{
	ourInstance = this;
}
