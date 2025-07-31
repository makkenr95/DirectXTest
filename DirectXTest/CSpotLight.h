#pragma once
#include "Vector4.hpp"

class CSpotLight
{
public:
	[[nodiscard]] CU::Vector4<float> GetPosition() const
	{
		return myPosition;
	}

	[[nodiscard]] CU::Vector4<float> GetDirection() const
	{
		return myDirection;
	}

	[[nodiscard]] CU::Vector4<float> GetColor() const
	{
		return myColor;
	}

	[[nodiscard]] float GetRange() const
	{
		return myRange;
	}

	[[nodiscard]] float GetInneAngle() const
	{
		return myInnerAngle;
	}

	[[nodiscard]] float GetOuterAngle() const
	{
		return myOuterAngle;
	}

	void SetPosition(const CU::Vector4<float>& aPosition)
	{
		myPosition = aPosition;
	}

	void SetDirection(const CU::Vector4<float>& aDirection)
	{
		myDirection = aDirection;
	}

	void SetColor(const CU::Vector4<float>& aColor)
	{
		myColor = aColor;
	}

	void SetRange(const float aRange)
	{
		myRange = aRange;
	}

	void SetInnerAngle(const float anInnerAngle)
	{
		myInnerAngle = anInnerAngle;
	}

	void SetOuterAngle(const float anOuterAngle)
	{
		myOuterAngle = anOuterAngle;
	}

private:
	CU::Vector4<float> myPosition;
	CU::Vector4<float> myDirection;
	CU::Vector4<float> myColor;
	float myRange = 1.f;
	float myInnerAngle = 1.f;
	float myOuterAngle = 1.f;
};

