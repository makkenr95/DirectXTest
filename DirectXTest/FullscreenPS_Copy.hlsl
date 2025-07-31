#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	returnValue.myColor.rgb = FullscreenTexture1.Sample(defaultSampler, input.myUV).rgb;
	returnValue.myColor.a = 1.0f;
	return returnValue;
}