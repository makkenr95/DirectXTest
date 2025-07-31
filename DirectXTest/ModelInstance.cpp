#include "pch.h"
#include "ModelInstance.h"

CModelInstance::CModelInstance()
{
}

CModelInstance::~CModelInstance()
{
}

void CModelInstance::SetTransform(CU::Vector3<float> aPosition, CU::Vector3<float> aRotation)
{
	SetPosition(aPosition);
	SetRotation(aRotation);
}

void CModelInstance::SetRotation(CU::Vector3<float> aRotation)
{
	const CU::Vector3<float> position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	const CU::Matrix4x4<float> rotation =
		CU::Matrix4x4<float>::CreateRotationAroundX(aRotation.x) *
		CU::Matrix4x4<float>::CreateRotationAroundY(aRotation.y) *
		CU::Matrix4x4<float>::CreateRotationAroundZ(aRotation.z);

	myTransform(1, 1) = rotation(1, 1);
	myTransform(1, 2) = rotation(1, 2);
	myTransform(1, 3) = rotation(1, 3);

	myTransform(2, 1) = rotation(2, 1);
	myTransform(2, 2) = rotation(2, 2);
	myTransform(2, 3) = rotation(2, 3);

	myTransform(3, 1) = rotation(3, 1);
	myTransform(3, 2) = rotation(3, 2);
	myTransform(3, 3) = rotation(3, 3);

	SetPosition(position);
}

void CModelInstance::SetPosition(CU::Vector3<float> aPosition)
{
	myTransform(4, 1) = aPosition.x;
	myTransform(4, 2) = aPosition.y;
	myTransform(4, 3) = aPosition.z;
	myTransform(4, 4) = 1.0f;
}

void CModelInstance::Move(CU::Vector3<float> aMovement)
{
	CU::Vector3<float> position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	position.x += aMovement.x;
	position.y += aMovement.y;
	position.z += aMovement.z;

	SetPosition(position);
}

void CModelInstance::Rotate(CU::Vector3<float> aRotation)
{
	const CU::Vector3<float> position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundX(aRotation.x);
	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundY(aRotation.y);
	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundZ(aRotation.z);

	SetPosition(position);
}

CModelInstance& CModelInstance::Init(std::vector<CModel*> aModels)
{
	myModel = aModels;
	return *this;
}

std::vector<CModel*> CModelInstance::GetModel() const
{
	return myModel;
}

CU::Matrix4x4<float> CModelInstance::GetTransform() const
{
	return myTransform;
}
