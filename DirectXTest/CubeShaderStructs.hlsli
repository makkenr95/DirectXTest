Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
TextureCube environmentTexture : register(t3);

SamplerState defaultSampler : register(s0);

struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myColor : COLOR;
	float2 myUV : UV;
	float4 myNormal : NORMAL;
	float4 myTangent : TANGENT;
	float4 myBinormal : BINORMAL;
};

struct VertexOutput
{
	float4 myPosition : SV_POSITION;
	float4 myWorldPosition : WORLDPOSITION;
	float4 myColor : COLOR;
	float2 myUV : UV;
	float4 myNormal : NORMAL;
	float4 myTangent : TANGENT;
	float4 myBinormal : BINORMAL;
};

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
float4x4 toCamera;
float4x4 toProjection;

float4 cameraPosition;

float4 toDirectionalLight;
float4 directionalLightColor;

int environmentLightMipCount;
int trash[3];
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 toWorld;
	float2 OB_UVScale;

	unsigned int OB_NumPointLight;
	unsigned int OB_NumSpotLight;

	struct PointLightData
	{
		float4 Position;
		float4 Color;
		float Range;
		float3 garbage;
	} myPointLights[8];

	struct my_struct
	{
		float4 Position;
		float4 Direction;
		float4 Color;
		float Range;
		float InnerAngle;
		float OuterAngle;
		float Trash;
	} mySpotLight[8];
}


// struct VertexInput
// {
//     float4 myPosition : POSITION;
//     float4 myColor : COLOR;
//     float4 myTangent : TANGENT;
//     float4 myBinormal : BINORMAL;
//     float4 myNormal : NORMAL;
//     float2 myUV : UV;
// };
//
// struct VertexToPixel
// {
//     float4 myPosition : SV_POSITION;
//     float4 myWorldPosition : WORLDPOSITION;
//     float4 myColor : COLOR;
//     float4 myTangent : TANGENT;
//     float4 myBinormal : BINORMAL;
//     float4 myNormal : NORMAL;
//     float2 myUV : UV;
// };
//
// struct PixelOutput
// {
//     float4 myColor : SV_TARGET;
// };
//
// cbuffer FrameBuffer : register(b0)
// {
//     float4x4 toCamera;
//     float4x4 toProjection;
//     float4 cameraPosition;
//     float4 toDirectionalLight;
//     float4 directionalLightColor;
// }
//
// cbuffer ObjectBuffer : register(b1)
// {
//     float4x4 toWorld;
// }
//
// Texture2D albedoTexture : register(t0);
// Texture2D normalTexture : register(t1);
// Texture2D materialTexture : register(t2);
// TextureCube environmentTexture : register(t3);
// SamplerState defaultSampler : register(s0);
