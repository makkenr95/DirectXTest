#pragma once
#include "Matrix4x4.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	class InputManager;
}

class CCamera
{
public:
	CCamera();
	~CCamera();

	bool Init(float aFov, CU::Vector2<float> aResolution);
	void Update(const float aDeltaTime);
	
	void SetTransform(CU::Vector3<float>, CU::Vector3<float> aRotation);
	void SetRotation(CU::Vector3<float> aRotation);
	void SetPosition(CU::Vector3<float> aPosition);
	void Move(CU::Vector3<float> aMovement);
	void Rotate(CU::Vector3<float> aRotation);

	CommonUtilities::Matrix4x4<float> GetTransform() const;
	CommonUtilities::Matrix4x4<float> GetProjection() const;
	CU::Vector3<float> GetPosition();
private:
	CommonUtilities::Matrix4x4<float> myTransform;
	CommonUtilities::Matrix4x4<float> myProjection;
	float myFarPlane = 10000.f;
	float myNearPlane = 1.f;
	CommonUtilities::InputManager* myInput;
	float myMoveSpeed = 300.f;
};

