#pragma once

struct ID3D11DeviceContext;

class FullscreenTexture
{
	friend class FullscreenTextureFactory;

	union 
	{
		struct ID3D11RenderTargetView* myRenderTarget;
		struct ID3D11DepthStencilView* myDepth;
	};

	struct ID3D11Texture2D* myTexture;
	struct ID3D11ShaderResourceView* mySRV;
	struct D3D11_VIEWPORT* myViewPort;

public:
	FullscreenTexture() = default;
	FullscreenTexture(ID3D11DeviceContext* aContext);
	~FullscreenTexture();

	void ClearTexture(CU::Vector4<float> aClearColor = { 0,0,0,0 });
	void ClearDepth(float aClearDepthValue = 1.0f, unsigned int aClearStencilValue = 0);
	void SetAsActiveTarget(FullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(unsigned int aSlot);
private:
	ID3D11DeviceContext* myContext;
};
