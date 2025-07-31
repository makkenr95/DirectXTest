#include "pch.h"
#include "EnvironmentLight.h"

CEnvironmentLight::~CEnvironmentLight()
{
	// delete myCubeMap;
	// myCubeMap = nullptr;
}

void CEnvironmentLight::SetDirection(CU::Vector4<float> aDirection)
{
	myDirection = aDirection;
}

void CEnvironmentLight::SetColor(CU::Vector4<float> aColor)
{
	myColor = aColor;
}

ID3D11ShaderResourceView* const* CEnvironmentLight::GetCubeMap()
{
	return &myCubeMap;
}

ID3D11ShaderResourceView* CEnvironmentLight::GetCubeMapNoConst()
{
	return myCubeMap;
}

CU::Vector4<float> CEnvironmentLight::GetDirection() const
{
	return myDirection;
}

CU::Vector4<float> CEnvironmentLight::GetColor() const
{
	return myColor;
}
