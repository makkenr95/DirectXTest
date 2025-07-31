#include "pch.h"
#include "Camera.h"
#include <cmath>
#include "DirectX11Framework.h"
#include "InputManager.h"
#include "MainSingleton.h"
# define M_PI 3.141592653589793238462643383279f

CCamera::CCamera()
{
	myTransform = CU::Matrix4x4<float>();
	myProjection = CU::Matrix4x4<float>();
}

CCamera::~CCamera()
{
}

bool CCamera::Init(float aFov, const CU::Vector2<float> aResolution)
{
	float hFoVRad = aFov * (M_PI / 180);
	float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (aResolution.y / aResolution.x));
	//float vFovDeg = std::ceil(vFoVRad * (180 / M_PI)); // Om man vill ha tillbaka till grader
	float myXScale = 1.f / std::tanf(hFoVRad * 0.5f);
	float myYScale = 1.f / std::tanf(vFoVRad * 0.5f);
	float Q = myFarPlane / (myFarPlane - myNearPlane);
	myProjection(1, 1) = myXScale;
	myProjection(2, 2) = myYScale;
	myProjection(3, 3) = Q;
	myProjection(3, 4) = 1.0f / Q;
	myProjection(4, 3) = -Q * myNearPlane;
	myProjection(4, 4) = 0.0f;
	myInput = MainSingleton::GetInstance()->GetInput();
	return true;
}

void CCamera::Update(const float aDeltaTime)
{
	CU::Vector3<float> velocity = {0,0,0};
	
	if (myInput->IsKeyDown('W') || myInput->IsKeyPressed('W'))
	{
		velocity += myTransform.GetForward() * myMoveSpeed * aDeltaTime;
	}
	if (myInput->IsKeyDown('S') || myInput->IsKeyPressed('S'))
	{
		velocity += myTransform.GetForward() * -myMoveSpeed * aDeltaTime;
	}
	if (myInput->IsKeyDown('D') || myInput->IsKeyPressed('D'))
	{
		velocity += myTransform.GetRight() * myMoveSpeed * aDeltaTime;
	}
	if (myInput->IsKeyDown('A') || myInput->IsKeyPressed('A'))
	{
		velocity += myTransform.GetRight() * -myMoveSpeed * aDeltaTime;
	}
	if (myInput->IsKeyDown('R') || myInput->IsKeyPressed('R'))
	{
		velocity += myTransform.GetUp() * myMoveSpeed * aDeltaTime;
	}
	if (myInput->IsKeyDown('F') || myInput->IsKeyPressed('F'))
	{
		velocity += myTransform.GetUp() * -myMoveSpeed  * aDeltaTime;
	}
	if (myInput->IsKeyDown('E') || myInput->IsKeyPressed('E'))
	{
		Rotate(CU::Vector3<float>{0,1,0} * aDeltaTime);
	}
	if (myInput->IsKeyDown('Q') || myInput->IsKeyPressed('Q'))
	{
		Rotate(CU::Vector3<float>{0,-1,0} * aDeltaTime);
	}
	Move(velocity);
}

void CCamera::SetTransform(CU::Vector3<float> aPosition, CU::Vector3<float> aRotation)
{
	SetPosition(aPosition);
	SetRotation(aRotation);
}

void CCamera::SetRotation(CU::Vector3<float> aRotation)
{
	const CU::Vector3 position = {
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

void CCamera::SetPosition(CU::Vector3<float> aPosition)
{
	myTransform(4, 1) = aPosition.x;
	myTransform(4, 2) = aPosition.y;
	myTransform(4, 3) = aPosition.z;
	myTransform(4, 4) = 1.0f;
}

void CCamera::Move(CU::Vector3<float> aMovement)
{
	CU::Vector3 position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	position.x += aMovement.x;
	position.y += aMovement.y;
	position.z += aMovement.z;

	SetPosition(position);
}

void CCamera::Rotate(CU::Vector3<float> aRotation)
{
	const CU::Vector3 position = {
		myTransform(4, 1),
		myTransform(4, 2),
		myTransform(4, 3)
	};

	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundX(aRotation.x);
	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundY(aRotation.y);
	myTransform *= CU::Matrix4x4<float>::CreateRotationAroundZ(aRotation.z);

	SetPosition(position);
}

CommonUtilities::Matrix4x4<float> CCamera::GetTransform() const
{
	return myTransform;
}

CommonUtilities::Matrix4x4<float> CCamera::GetProjection() const
{
	return myProjection;
}

CU::Vector3<float> CCamera::GetPosition()
{
	return { myTransform(4,1), myTransform(4,2), myTransform(4,3) };
}
