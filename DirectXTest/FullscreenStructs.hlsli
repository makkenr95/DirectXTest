Texture2D FullscreenTexture1 : register(t0);
Texture2D FullscreenTexture2 : register(t1);
SamplerState defaultSampler : register(s0);

static const float2 Resolution = float2(1600.0f, 900.0f) / 4.0f;
static const float GaussianKernel5[5] = { .06136f, .24477f, .38774f, .24477f, .06136f };

struct VertexInput
{
	unsigned int myIndex : SV_VertexID;
};

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
	float2 myUV : UV;
};

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};
