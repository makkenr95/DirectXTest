#include "pch.h"
#include "ModelFactory.h"
#include <fstream>
#include "DirectX11Framework.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

CModelFactory* CModelFactory::ourInstance = nullptr;

CModelFactory::CModelFactory()
{
	ourInstance = this;
}


CModelFactory::~CModelFactory()
{
	ourInstance = nullptr;
}

std::vector<CModel*> CModelFactory::GetModel(std::string aFilePath)
{
	HRESULT result;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aFilePath,
	                                         aiProcess_CalcTangentSpace |
	                                         aiProcess_Triangulate |
	                                         aiProcess_JoinIdenticalVertices |
	                                         aiProcess_SortByPType |
	                                         aiProcess_ConvertToLeftHanded);

	if (!scene)
	{
		assert(false && "Failed");
	}

	std::vector<CModel*> models;

	aiMesh** meshses = scene->mMeshes;
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		auto mesh = meshses[i];

		const int numberOfVertices = mesh->mNumVertices;
		const int numberOfFaces = mesh->mNumFaces;

		auto positions = mesh->mVertices;
		//auto colors =  mesh->mColors;
		auto UV = *mesh->mTextureCoords;
		auto faces = mesh->mFaces;
		auto normals = mesh->mNormals;
		auto tangents = mesh->mTangents;
		auto bitangents = mesh->mBitangents;

		struct Vertex
		{
			float x, y, z, w; // Positions
			//float wx, wy, wz, ww;	// World Positions
			float r, g, b, a; // Color
			float u, v; // UV
			float nx, ny, nz, nw; // Normal
			float tx, ty, tz, tw; // Tangent
			float bx, by, bz, bw; // Binormal
		};

		std::vector<Vertex> vertices;
		vertices.reserve(numberOfVertices);
		vertices.resize(numberOfVertices);

		for (int vertexIndex = 0; vertexIndex < numberOfVertices; ++vertexIndex)
		{
			vertices[vertexIndex].x = positions[vertexIndex].x;
			vertices[vertexIndex].y = positions[vertexIndex].y;
			vertices[vertexIndex].z = positions[vertexIndex].z;
			vertices[vertexIndex].w = 1.0f;

			//vertices[vertexIndex].wx = positions[vertexIndex].x;
			//vertices[vertexIndex].wy = positions[vertexIndex].y;
			//vertices[vertexIndex].wz = positions[vertexIndex].z;
			//vertices[vertexIndex].ww = 1.0f;

			// vertices[vertexIndex].r = colors[vertexIndex].x;
			// vertices[vertexIndex].g = colors[vertexIndex].y;
			// vertices[vertexIndex].b = colors[vertexIndex].z;
			// vertices[vertexIndex].a = 1.f;

			vertices[vertexIndex].u = UV[vertexIndex].x;
			vertices[vertexIndex].v = UV[vertexIndex].y;

			vertices[vertexIndex].nx = normals[vertexIndex].x;
			vertices[vertexIndex].ny = normals[vertexIndex].y;
			vertices[vertexIndex].nz = normals[vertexIndex].z;
			vertices[vertexIndex].nw = 0.0f;

			vertices[vertexIndex].tx = tangents[vertexIndex].x;
			vertices[vertexIndex].ty = tangents[vertexIndex].y;
			vertices[vertexIndex].tz = tangents[vertexIndex].z;
			vertices[vertexIndex].tw = 0.0f;

			vertices[vertexIndex].bx = bitangents[vertexIndex].x;
			vertices[vertexIndex].by = bitangents[vertexIndex].y;
			vertices[vertexIndex].bz = bitangents[vertexIndex].z;
			vertices[vertexIndex].bw = 0.0f;
		}

		std::vector<unsigned int> indices;
		indices.reserve(numberOfFaces * 3);
		indices.resize(numberOfFaces * 3);

		int faceIndex = 0;
		for (int indicesIndex = 0; indicesIndex < indices.size();)
		{
			for (int index = 0; index < 3; ++index)
			{
				indices[indicesIndex] = faces[faceIndex].mIndices[index];
				indicesIndex++;
			}
			faceIndex++;
		}

		D3D11_BUFFER_DESC vertexBufferDescription = {0};
		// vertexBufferDescription.ByteWidth = sizeof vertices;
		vertexBufferDescription.ByteWidth = static_cast<UINT>(vertices.size()) * sizeof vertices[0];
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {nullptr};
		vertexSubresourceData.pSysMem = vertices.data();


		ID3D11Buffer* vertexBuffer;
		result = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}
		D3D11_BUFFER_DESC indexBufferDescription = {0};
		indexBufferDescription.ByteWidth = static_cast<UINT>(indices.size()) * sizeof indices[0];
		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {nullptr};
		indexSubresourceData.pSysMem = indices.data();


		ID3D11Buffer* indexBuffer;
		result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}
		///End Vertex

		///Start Shader
		std::ifstream vsFile;
		vsFile.open("CubeVertexShader.cso", std::ios::binary);
		std::string vsData = {std::istreambuf_iterator(vsFile), std::istreambuf_iterator<char>()};
		ID3D11VertexShader* vertexShader;
		result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}

		std::ifstream psFile;
		psFile.open("CubePixelShader.cso", std::ios::binary);
		std::string psData = {std::istreambuf_iterator(psFile), std::istreambuf_iterator<char>()};
		ID3D11PixelShader* pixelShader;
		result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			//{"WORLDPOSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{
				"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
		};

		ID3D11InputLayout* inputLayout;
		result = myDevice->CreateInputLayout(layout, sizeof layout / sizeof D3D11_INPUT_ELEMENT_DESC, vsData.data(),
		                                     vsData.size(), &inputLayout);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}

		//Start Textures
		std::string texturePath = aFilePath;
		std::wstring wTexturePath(texturePath.begin(), texturePath.end() - 4);
		wTexturePath.append(L"_D.dds");
		ID3D11ShaderResourceView* shaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, wTexturePath.c_str(), nullptr, &shaderResourceView);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}
		//End Textures

	//Start Normal
		std::string normalTexturePath = aFilePath;
		std::wstring wNormalTexturePath(normalTexturePath.begin(), normalTexturePath.end() - 4);
		wNormalTexturePath.append(L"_N.dds");
		ID3D11ShaderResourceView* normalShaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, wNormalTexturePath.c_str(), nullptr, &normalShaderResourceView);
		if (FAILED(result))
		{
			assert(false && "Failed");
		}
		//End Normal

		//Start Material
		std::string materialPath = aFilePath;
		std::wstring materialTexturePath(materialPath.begin(), materialPath.end() - 4);
		materialTexturePath.append(L"_M.dds");
		ID3D11ShaderResourceView* materialShaderResourceView;
		result = DirectX::CreateDDSTextureFromFile(myDevice, materialTexturePath.c_str(), nullptr, &materialShaderResourceView);
		if (FAILED(result))
		{
			std::wstring materialTexturePathNew(materialPath.begin(), materialPath.end() - 4);
			materialTexturePathNew.append(L"_r.dds");
			result = DirectX::CreateDDSTextureFromFile(myDevice, materialTexturePathNew.c_str(), nullptr, &materialShaderResourceView);
			// If head model, wrong texture is loaded
			assert("Failed");
		}
		//End Textures

		CModel* model = new CModel();
		if (!model)
		{
			assert(false && "Failed");
		}

		CModel::SModelData modelData;
		modelData.myNumberOfVertecies = numberOfVertices;
		modelData.myNumberOfIndices = numberOfFaces * 3;
		modelData.myStride = sizeof Vertex;
		modelData.myOffset = 0;
		modelData.myVertexBuffer = vertexBuffer;
		modelData.myIndexBuffer = indexBuffer;
		modelData.myVertexShader = vertexShader;
		modelData.myPixelShader = pixelShader;
		modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelData.myInputLayout = inputLayout;
		modelData.myTexture[0] = shaderResourceView;
		modelData.myTexture[1] = normalShaderResourceView;
		modelData.myTexture[2] = materialShaderResourceView;
		model->Init(modelData);
		models.push_back(model);
	}
	return models;
}

void CModelFactory::SetDevice(ID3D11Device* aDevice)
{
	myDevice = aDevice;
}

CModelFactory* CModelFactory::GetInstance()
{
	if (!ourInstance)
	{
		ourInstance = new CModelFactory;
	}
	return ourInstance;
}

CModel* CModelFactory::LoadModel(std::string aFilePath)
{
	HRESULT result;

	//Start Vertex
	struct Vertex
	{
		float x, y, z, w;
		float r, g, b, a;
	} vertecies[3] =
		{
			{-0.8f, -0.8f, 0, 1, 1, 0, 0, 1},
			{0.0f, 0.8f, 0, 1, 0, 1, 0, 1},
			{0.8f, -0.8f, 0, 1, 0, 0, 1, 1}
		};

	D3D11_BUFFER_DESC bufferDescription = {0};
	bufferDescription.ByteWidth = sizeof vertecies;
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData = {0};
	subresourceData.pSysMem = vertecies;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	//End Vertex

	//Start Shader
	std::ifstream vsFile;
	vsFile.open("VertexShader.cso", std::ios::binary);
	std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		return nullptr;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("PixelShader.cso", std::ios::binary);
	std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		return nullptr;
	}
	psFile.close();
	//End Shader

	//Start Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return nullptr;
	}
	//End Layout

	CModel* model = new CModel();
	if (!model)
	{
		return nullptr;
	}

	CModel::SModelData modelData;
	modelData.myNumberOfVertecies = sizeof vertecies / sizeof Vertex;
	modelData.myStride = sizeof Vertex;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	model->Init(modelData);
	return model;
}

CModel* CModelFactory::GetCube() const
{
	HRESULT result;

	///Start Vertex

	struct Vertex
	{
		float x, y, z, w;
		float r, g, b, a;
		float u, v;
	} vertices[8] =
		{
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{1, 1, -1, 1, 1, 1, 0, 1, 1, 0},
			{1, -1, 1, 1, 1, 0, 1, 1, 1, 1},
			{-1, 1, 1, 1, 0, 1, 1, 1, 0, 0},
			{1, -1, -1, 1, 1, 0, 0, 1, 1, 1},
			{-1, 1, -1, 1, 0, 1, 0, 1, 0, 0},
			{-1, -1, 1, 1, 0, 0, 1, 1, 0, 1},
			{-1, -1, -1, 1, 0, 0, 0, 1, 0, 1}

		};
	unsigned int indices[36] =
	{
		0, 2, 1,
		0, 1, 3,
		0, 3, 2,
		1, 2, 4,
		2, 3, 6,
		3, 1, 5,
		4, 5, 1,
		5, 6, 3,
		6, 4, 2,
		7, 6, 5,
		7, 5, 4,
		7, 4, 6
	};
	D3D11_BUFFER_DESC vertexBufferDescription = {0};
	vertexBufferDescription.ByteWidth = sizeof vertices;
	vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {nullptr};
	vertexSubresourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	D3D11_BUFFER_DESC indexBufferDescription = {0};
	indexBufferDescription.ByteWidth = sizeof indices;
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexSubresourceData = {nullptr};
	indexSubresourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}
	///End Vertex

	///Start Shader
	std::ifstream vsFile;
	vsFile.open("CubeVertexShader.cso", std::ios::binary);
	std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		return nullptr;
	}

	std::ifstream psFile;
	psFile.open("CubePixelShader.cso", std::ios::binary);
	std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		return nullptr;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, sizeof layout / sizeof D3D11_INPUT_ELEMENT_DESC, vsData.data(),
	                                     vsData.size(), &inputLayout);
	if (FAILED(result))
	{
		return nullptr;
	}

	//Start Textures
	std::wstring filename = L"a.dds";
	ID3D11ShaderResourceView* shaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &shaderResourceView);
	if (FAILED(result))
	{
		return nullptr;
	}
	//End Textures

	CModel* model = new CModel();
	if (!model)
	{
		return nullptr;
	}

	CModel::SModelData modelData;
	modelData.myNumberOfVertecies = sizeof vertices / sizeof Vertex;
	modelData.myNumberOfIndices = sizeof indices / sizeof(unsigned int);
	modelData.myStride = sizeof Vertex;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = shaderResourceView;
	model->Init(modelData);
	return model;
}
