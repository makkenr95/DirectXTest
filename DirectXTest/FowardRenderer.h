#pragma once
#include <d3d11.h>
#include <vector>
#include "Matrix4x4.hpp"
#include "Vector2.hpp"

class CSpotLight;
class CPointLight;
class CEnvironmentLight;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
class CDirectX11Framework;
class CModelInstance;
class CCamera;

class CFowardRenderer
{
public:
	CFowardRenderer();
	~CFowardRenderer();
	bool Init(CDirectX11Framework* aFramework);
	void Render(CCamera* aCamera, std::vector<CPointLight*> somePointLights, std::vector<CSpotLight*> someSpotLights, CEnvironmentLight* aEnvironmentLight, std::vector<CModelInstance*>& aModelList);
	void Render(CCamera* aCamera, CEnvironmentLight* aEnvironmentLight, std::vector<CModelInstance*>& aModelList);
private:
	struct FrameBufferData
	{
		CU::Matrix4x4<float> myToCamera;
		CU::Matrix4x4<float> myToProjection;
		CU::Vector4<float> myCameraPosition;
		CU::Vector4<float> myDirectionalLightDirection;
		CU::Vector4<float> myDirectionalLightColor;
		unsigned myEnvironmentLightMipCount;
		CU::Vector3<unsigned> trash;
	} myFrameBufferData;

	struct ObjectBufferData
	{
		CU::Matrix4x4<float> myToWorld;

		CU::Vector2<float> myUVScale;
		unsigned int myNumPointLights;
		unsigned int myNumSpotLights;

		struct PointLightData
		{
			CU::Vector4<float> myPosition;
			CU::Vector4<float> myColorAndIntensity;
			float myRange;
			CU::Vector3<float> GAAAARBage;
		} myPointLights[8];

		struct SpotLightData
		{
			CU::Vector4<float> myPosition;
			CU::Vector4<float> myDirection;
			CU::Vector4<float> myColorAndIntensity;
			float myRange;
			float myInnerAngle;
			float myOuterAngle;
			float myTRRRASH;
		} mySpotLights[8];

	} myObjectBufferData;
	
	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
};

