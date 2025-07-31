#include "pch.h"
#include "FowardRenderer.h"
#include "Camera.h"
#include "CPointLight.h"
#include "CSpotLight.h"
#include "DirectX11Framework.h"
#include "EnvironmentLight.h"
#include "Model.h"
#include "ModelInstance.h"

CFowardRenderer::CFowardRenderer()
{
}

CFowardRenderer::~CFowardRenderer()
{
}

bool CFowardRenderer::Init(CDirectX11Framework* aFramework)
{
	if (!aFramework)
	{
		return false;
	}

	myContext = aFramework->GetContext();
	if (!myContext)
	{
		return false;
	}

	ID3D11Device* device = aFramework->GetDevice();
	if (!device)
	{
		return false;
	}
	HRESULT result;

	D3D11_BUFFER_DESC bufferDesctiption = { 0 };
	bufferDesctiption.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesctiption.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesctiption.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesctiption.ByteWidth = sizeof FrameBufferData;
	result = device->CreateBuffer(&bufferDesctiption, nullptr, &myFrameBuffer);
	if (FAILED(result))
	{
		return false;
	}

	bufferDesctiption.ByteWidth = sizeof ObjectBufferData;
	result = device->CreateBuffer(&bufferDesctiption, nullptr, &myObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void CFowardRenderer::Render(CCamera* aCamera, std::vector<CPointLight*> somePointLights, std::vector<CSpotLight*> someSpotLights, CEnvironmentLight* aEnvironmentLight,std::vector<CModelInstance*>& aModelList)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;
	
	myFrameBufferData.myToCamera = aCamera->GetTransform();
	myFrameBufferData.myToCamera = CU::Matrix4x4<float>().GetFastInverse(myFrameBufferData.myToCamera);
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myDirectionalLightColor = aEnvironmentLight->GetColor();
	myFrameBufferData.myDirectionalLightDirection = aEnvironmentLight->GetDirection();

	myFrameBufferData.myCameraPosition = {
		aCamera->GetPosition().x,
		aCamera->GetPosition().y,
		aCamera->GetPosition().z,
		1.0f
	};


	for (int k = 0; k < somePointLights.size(); ++k)
	{
		const auto& pointLight = somePointLights[k];
		myObjectBufferData.myPointLights[k].myRange = pointLight->GetRange();
		myObjectBufferData.myPointLights[k].myPosition = pointLight->GetPosition();
		myObjectBufferData.myPointLights[k].myColorAndIntensity = pointLight->GetColor();
	}
	myObjectBufferData.myNumPointLights = static_cast<unsigned>(somePointLights.size());

	for (int j = 0; j < someSpotLights.size(); ++j)
	{
		const CSpotLight* spotLight = someSpotLights[j];
		myObjectBufferData.mySpotLights[j].myPosition = spotLight->GetPosition();
		myObjectBufferData.mySpotLights[j].myRange = spotLight->GetRange();
		myObjectBufferData.mySpotLights[j].myColorAndIntensity = spotLight->GetColor();
		myObjectBufferData.mySpotLights[j].myDirection = spotLight->GetDirection();
		myObjectBufferData.mySpotLights[j].myInnerAngle = spotLight->GetInneAngle();
		myObjectBufferData.mySpotLights[j].myOuterAngle = spotLight->GetOuterAngle();
	}
	myObjectBufferData.myNumSpotLights = static_cast<unsigned>(someSpotLights.size());

	//ID3D11Resource* cubeResource = nullptr;
	//aEnvironmentLight->GetCubeMap()->GetResource(&cubeResource);
	//ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
	//D3D11_TEXTURE2D_DESC cubeDescription = {};
	//cubeTexture->GetDesc(&cubeDescription);
	//myFrameBufferData.myEnvironmentLightMipCount = cubeDescription.MipLevels;
	//cubeResource->Release();

	ID3D11Resource* cubeResource = nullptr;
	aEnvironmentLight->GetCubeMapNoConst()->GetResource(&cubeResource);
	ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
	D3D11_TEXTURE2D_DESC cubeDescription = {};
	cubeTexture->GetDesc(&cubeDescription);
	myFrameBufferData.myEnvironmentLightMipCount = cubeDescription.MipLevels;
	cubeResource->Release();
	
	ZeroMemory(&bufferData, sizeof D3D11_MAPPED_SUBRESOURCE);
	result = myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof FrameBufferData);
	//myContext->Unmap(myFrameBuffer, 0);
	//myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	//myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	//myContext->PSSetShaderResources(3, 0, aEnvironmentLight->GetCubeMap());


	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetShaderResources(3, 1, aEnvironmentLight->GetCubeMap());

	for (const CModelInstance* instance : aModelList)
	{
		std::vector<CModel*> model = instance->GetModel();

		for (int i = 0; i < model.size(); ++i)
		{
			

		CModel::SModelData modelData = model[i]->GetModelData();
		
	
		myObjectBufferData.myToWorld = instance->GetTransform();
		ZeroMemory(&bufferData, sizeof D3D11_MAPPED_SUBRESOURCE);
		result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result))
		{
			return;
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof ObjectBufferData);
		myContext->Unmap(myObjectBuffer, 0);
		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);
		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);
		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		//myContext->PSSetShaderResources(0, 3, &modelData.myTexture[0]);
		//myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->PSSetShaderResources(0, 3, &modelData.myTexture[0]);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);
		
		myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
		}
	}
}

void CFowardRenderer::Render(CCamera* aCamera, CEnvironmentLight* aEnvironmentLight,
	std::vector<CModelInstance*>& aModelList)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.myToCamera = aCamera->GetTransform();
	myFrameBufferData.myToCamera = CU::Matrix4x4<float>().GetFastInverse(myFrameBufferData.myToCamera);
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myDirectionalLightColor = aEnvironmentLight->GetColor();
	myFrameBufferData.myDirectionalLightDirection = aEnvironmentLight->GetDirection();

	myFrameBufferData.myCameraPosition = {
		aCamera->GetPosition().x,
		aCamera->GetPosition().y,
		aCamera->GetPosition().z,
		1.0f
	};

	ID3D11Resource* cubeResource = nullptr;
	aEnvironmentLight->GetCubeMapNoConst()->GetResource(&cubeResource);
	ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
	D3D11_TEXTURE2D_DESC cubeDescription = {};
	cubeTexture->GetDesc(&cubeDescription);
	myFrameBufferData.myEnvironmentLightMipCount = cubeDescription.MipLevels;
	cubeResource->Release();

	ZeroMemory(&bufferData, sizeof D3D11_MAPPED_SUBRESOURCE);
	result = myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result))
	{
		return;
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof FrameBufferData);

	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetShaderResources(3, 1, aEnvironmentLight->GetCubeMap());

	for (const CModelInstance* instance : aModelList)
	{
		std::vector<CModel*> model = instance->GetModel();

		for (int i = 0; i < model.size(); ++i)
		{


			CModel::SModelData modelData = model[i]->GetModelData();


			myObjectBufferData.myToWorld = instance->GetTransform();
			ZeroMemory(&bufferData, sizeof D3D11_MAPPED_SUBRESOURCE);
			result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if (FAILED(result))
			{
				return;
			}

			memcpy(bufferData.pData, &myObjectBufferData, sizeof ObjectBufferData);
			myContext->Unmap(myObjectBuffer, 0);
			myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
			myContext->IASetInputLayout(modelData.myInputLayout);
			myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
			myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
			myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);
			myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);

			myContext->PSSetShaderResources(0, 3, &modelData.myTexture[0]);
			myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

			myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
		}
	}
}
	