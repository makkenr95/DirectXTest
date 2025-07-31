#include "pch.h"
#include "FullscreenTexture.h"
#include <d3d11.h>

FullscreenTexture::FullscreenTexture(ID3D11DeviceContext* aContext)
{
	myContext = aContext;
}

FullscreenTexture::~FullscreenTexture()
{
}

void FullscreenTexture::ClearTexture(CU::Vector4<float> aClearColor)
{
	myContext->ClearRenderTargetView(myRenderTarget, &aClearColor.x);
}

void FullscreenTexture::ClearDepth(float aClearDepthValue, unsigned aClearStencilValue)
{
	myContext->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, aClearDepthValue, static_cast<UINT8>(aClearStencilValue));
}

void FullscreenTexture::SetAsActiveTarget(FullscreenTexture* aDepth)
{
	if (aDepth)
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, aDepth->myDepth);
	}
	else
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, nullptr);
	}
	myContext->RSSetViewports(1, myViewPort);
}

void FullscreenTexture::SetAsResourceOnSlot(unsigned aSlot)
{
	myContext->PSSetShaderResources(aSlot, 1, &mySRV);
}
