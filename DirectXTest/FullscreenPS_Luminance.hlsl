#include "FullscreenStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;

	const float3 resource = FullscreenTexture1.Sample(defaultSampler, input.myUV).rgb;

	//Gain no cutoff
	//{
	//	const float luminance = dot(resource, float3(0.2126f, .7152f, .0722f));
	//	returnValue.myColor.rgb = resource * pow(luminance, 5);
	//}
	//
	////Flat cutoff
	//{
	//	const float luminance = dot(resource, float3(0.2126f, .7152f, .0722f));
	//	const float cutOff = .8f;
	//	if (luminance >= cutOff)
	//		returnValue.myColor.rgb = resource;
	//	else
	//		returnValue.myColor.rgb = 0;
	//}
	//
	////Gain cutoff
	//{
	//	float luminance = dot(resource, float3(0.2126f, .7152f, .0722f));
	//	const float cutOff = .5f;
	//	luminance = saturate(luminance - cutOff);
	//	returnValue.myColor.rgb = resource * luminance * (1.0f / cutOff);
	//}

	//Flat cutoff with smooth edges
	{
		const float luminance = dot(resource, float3(0.2126f, .7152f, .0722f));
		const float cutOff = .8f;
		if (luminance >= cutOff)
		{
			returnValue.myColor.rgb = resource;
		}
		else if (luminance >= cutOff * .5f)
		{
			float fade = luminance / cutOff;
			fade = pow(fade, 5);
			returnValue.myColor.rgb = resource * fade;
		}
		else
		{
			returnValue.myColor.rgb = 0;
		}
	}
	returnValue.myColor.a = 1.0f;
	return returnValue;
}