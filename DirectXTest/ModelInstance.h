#pragma once
#include "Model.h"
#include "Matrix4x4.hpp"
#include "Vector2.hpp"

class CModelInstance
{
public:
	CModelInstance();
	~CModelInstance();
	CModelInstance& Init(std::vector<CModel*> aModels);
	void SetTransform(CU::Vector3<float>, CU::Vector3<float> aRotation);
	void SetRotation(CU::Vector3<float> aRotation);
	void SetPosition(CU::Vector3<float> aPosition);
	void Move(CU::Vector3<float> aMovement);
	void Rotate(CU::Vector3<float> aRotation);

	std::vector<CModel*> GetModel() const;
	CU::Matrix4x4<float> GetTransform() const;
private:
	std::vector<CModel*> myModel;
	CU::Matrix4x4<float> myTransform;
};

