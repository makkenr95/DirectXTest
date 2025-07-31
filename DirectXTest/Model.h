#pragma once
#include "DirectX11Framework.h"

class CModel
{
public:
	struct SModelData
	{
		UINT myNumberOfVertecies = 0;
		UINT myNumberOfIndices = 0;
		UINT myStride = 0;
		UINT myOffset = 0;
		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;
		std::array<ID3D11ShaderResourceView*, 3> myTexture = { nullptr };
	};
	CModel& Init(SModelData aModelData);
	SModelData GetModelData() const;
private:
	SModelData myModelData = {};
};

