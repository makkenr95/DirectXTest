#pragma once
#include <d3d11.h>
#include "Vector4.hpp"

class CEnvironmentLight
{
public:
	CEnvironmentLight() = delete;
	CEnvironmentLight(ID3D11ShaderResourceView* aShaderResourceView) : myCubeMap(aShaderResourceView) {}
	~CEnvironmentLight();
	void SetDirection(CU::Vector4<float> aDirection);
	void SetColor(CU::Vector4<float> aColor);
	ID3D11ShaderResourceView* const* GetCubeMap();
	ID3D11ShaderResourceView* GetCubeMapNoConst();
	[[nodiscard]] CU::Vector4<float> GetDirection() const;
	[[nodiscard]] CU::Vector4<float> GetColor() const;
private:
	CU::Vector4<float> myDirection;
	CU::Vector4<float> myColor;
	ID3D11ShaderResourceView* myCubeMap = nullptr;
};
