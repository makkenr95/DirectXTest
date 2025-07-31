#pragma once
#include "Matrix4x4.hpp"

class CPointLight
{
public:
	void SetPosition(const CU::Vector3<float>& aPosition);
	void SetColor(const CU::Vector4<float>& aColor);
	void SetRange(float aRange);

	[[nodiscard]] CommonUtilities::Vector4<float> GetPosition() const
	{
		return {myPosition.x, myPosition.y, myPosition.z, 0.0f};
	}

	[[nodiscard]] CU::Vector4<float> GetColor() const
	{
		return myColor;
	}

	[[nodiscard]] float GetRange() const
	{
		return myRange;
	}

private:
	CU::Vector3<float> myPosition;
	CU::Vector4<float> myColor;
	float myRange = 1.0f;
};
