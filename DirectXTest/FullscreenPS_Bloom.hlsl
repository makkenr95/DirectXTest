#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;

	float3 resource1 = FullscreenTexture1.Sample(defaultSampler, input.myUV).rgb;
	float3 resource2 = FullscreenTexture2.Sample(defaultSampler, input.myUV).rgb;

	//Additive
	//{
	//	returnValue.myColor.rgb = resource1 + resource2;
	//}
	
	//Scaled
	//{
	//	float3 scaledResource = resource1 * (1.0f - saturate(resource2));
	//	returnValue.myColor.rgb = scaledResource + resource2;
	//}

	//Luminance based
	{
		float luminance = dot(resource1, float3(0.2126f, .75152, .0722f));
		float3 scaledResource = resource2 * (1.0f - luminance);
		returnValue.myColor.rgb = resource1 + scaledResource;
	}

	returnValue.myColor.a = 1.0f;
	return returnValue;
}