#pragma once
#include "FullscreenTexture.h"
#include <dxgiformat.h>

struct ID3D11Device;

class FullscreenTextureFactory
{
public:
	FullscreenTextureFactory(ID3D11DeviceContext* aContext, ID3D11Device* aDevice);
	FullscreenTexture CreateTexture(CU::Vector2<unsigned> aSize, DXGI_FORMAT aFormat);
	FullscreenTexture CreateTexture(ID3D11Texture2D* aTextureTemplate);
	FullscreenTexture CreateDepth(CU::Vector2<unsigned> aSize, DXGI_FORMAT aFormat);
private:
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
};
