#include "pch.h"
#include "CPointLight.h"

void CPointLight::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
}

void CPointLight::SetColor(const CU::Vector4<float>& aColor)
{
	myColor = aColor;
}

void CPointLight::SetRange(const float aRange)
{
	myRange = aRange;
}
