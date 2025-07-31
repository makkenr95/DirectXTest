#include "pch.h"
#include "FullscreenRender.h"
#include <fstream>
#include <d3d11.h>
#include <filesystem>

bool FullscreenRender::Init(ID3D11DeviceContext* aContext, ID3D11Device* aDevice)
{
	myContext = aContext;
	myDevice = aDevice;
	HRESULT result;

	//std::ifstream vsFilez;
	//vsFilez.open("CubeVertexShader.cso", std::ios::binary);
	//std::string vsDataz = { std::istreambuf_iterator(vsFilez), std::istreambuf_iterator<char>() };
	//ID3D11VertexShader* vertexShader;
	//result = myDevice->CreateVertexShader(vsDataz.data(), vsDataz.size(), nullptr, &vertexShader);
	//if (FAILED(result))
	//{
	//	assert(false && "Failed");
	//}


	std::ifstream vsFile;
	vsFile.open("FullscreenVS.cso", std::ios::binary);
	if (vsFile.fail())
	{
		assert(false && "Cant find file");
	}
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader);
	vsFile.close();
	if (FAILED(result))
		return false;

	std::array<std::string, Shader::COUNT> shaderPaths;
	shaderPaths[Shader::COPY] = "FullscreenPS_Copy.cso";
	shaderPaths[Shader::LUMINACE] = "FullscreenPS_Luminance.cso";
	shaderPaths[Shader::GAUSSIANHORIZONTAL] = "FullscreenPS_GaussianH.cso";
	shaderPaths[Shader::GAUSSIANVERTICAL] = "FullscreenPS_GaussianV.cso";
	shaderPaths[Shader::BLOOM] = "FullscreenPS_Bloom.cso";

	for (int i = 0; i < Shader::COUNT; ++i)
	{
		std::ifstream psFile;
		psFile.open(shaderPaths[i], std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShaders[i]);
		psFile.close();
		if (FAILED(result))
			return false;
	}
	return true;
}

void FullscreenRender::Render(const Shader aMode)
{
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(nullptr);
	myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	myContext->VSSetShader(myVertexShader, nullptr, 0);
	myContext->PSSetShader(myPixelShaders[aMode], nullptr, 0);
	myContext->Draw(3, 0);
}
