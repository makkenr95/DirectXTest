#pragma once

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Device;
struct ID3D11DeviceContext;

class FullscreenRender
{
public:
	enum Shader
	{
		COPY,
		LUMINACE,
		GAUSSIANHORIZONTAL,
		GAUSSIANVERTICAL,
		BLOOM,
		COUNT
	};
	bool Init(ID3D11DeviceContext* aContext, ID3D11Device* aDevice);
	void Render(Shader aMode);

private:
	ID3D11VertexShader* myVertexShader;
	std::array<ID3D11PixelShader*, Shader::COUNT> myPixelShaders;
	ID3D11Device* myDevice = nullptr;
	ID3D11DeviceContext* myContext = nullptr;
};

