#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	float texelSize = 1.0f / Resolution.y;
	float3 blurColor = 0;
	const unsigned int kernelSize = 5;
	const float start = ((float)kernelSize - 1.0f) / 2.0f * -1.0f;
	for (int idx = 0; idx < kernelSize; ++idx)
	{
		const float2 uv = input.myUV + float2(texelSize * (start + (float)idx), 0.0f);
		const float3 resource = FullscreenTexture1.Sample(defaultSampler, uv).rgb;
		blurColor += resource * GaussianKernel5[idx];
	}

	returnValue.myColor.rgb = blurColor;
	returnValue.myColor.a = 1.0f;

	return returnValue;
}