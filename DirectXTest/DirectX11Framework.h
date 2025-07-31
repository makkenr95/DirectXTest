#pragma once
#include <array>
#include "WindowHandler.h"
#include <d3d11.h>

class CDirectX11Framework
{
public:
	CDirectX11Framework();
	~CDirectX11Framework();

	bool Init(CWindowHandler* aWindowHandler);
	void BeginFrame(std::array<float, 4>& aClearColor);
	void EndFrame();

	IDXGISwapChain* GetSwapChain() const;
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	ID3D11RenderTargetView* GetBackBuffer() const;
private:
	IDXGISwapChain* mySwapChain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	ID3D11RenderTargetView* myBackBuffer;
	ID3D11DepthStencilView* myDepthBuffer;
};
