#include "pch.h"
#include "DirectX11Framework.h"

CDirectX11Framework::CDirectX11Framework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
	myBackBuffer = nullptr;
	myDepthBuffer = nullptr;
}

CDirectX11Framework::~CDirectX11Framework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
	myBackBuffer = nullptr;
	myDepthBuffer = nullptr;
}

bool CDirectX11Framework::Init(CWindowHandler* aWindowHandler)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = aWindowHandler->GetWindowHandle();
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.Windowed = true;

	result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
		&swapChainDescription, &mySwapChain, &myDevice, nullptr, &myContext);

	if (FAILED(result))
	{
		return false;
	}

	ID3D11Texture2D* backbufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateRenderTargetView(backbufferTexture, nullptr, &myBackBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = backbufferTexture->Release();
	if (FAILED(result))
	{
		return false;
	}

	//TEMP
	ID3D11Texture2D* depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDescription = { 0 };
	depthBufferDescription.Width = static_cast<unsigned>(aWindowHandler->GetWidth());
	depthBufferDescription.Height = static_cast<unsigned>(aWindowHandler->GetHeight());
	depthBufferDescription.ArraySize = 1;
	depthBufferDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDescription.SampleDesc.Count = 1;
	depthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	result = myDevice->CreateTexture2D(&depthBufferDescription, nullptr, &depthBufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
	if (FAILED(result))
	{
		return false;
	}

	myContext->OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);
	
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aWindowHandler->GetWidth());
	viewport.Height = static_cast<float>(aWindowHandler->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);
	//TEMP

	return true;
}

void CDirectX11Framework::BeginFrame(std::array<float, 4>& aClearColor)
{
	myContext->ClearRenderTargetView(myBackBuffer, &aClearColor[0]);
	myContext->ClearDepthStencilView(myDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDirectX11Framework::EndFrame()
{
	mySwapChain->Present(1, 0);
}

IDXGISwapChain* CDirectX11Framework::GetSwapChain() const
{
	return mySwapChain;
}

ID3D11Device* CDirectX11Framework::GetDevice() const
{
	return myDevice;
}

ID3D11DeviceContext* CDirectX11Framework::GetContext() const
{
	return myContext;
}

ID3D11RenderTargetView* CDirectX11Framework::GetBackBuffer() const
{
	return myBackBuffer;
}
