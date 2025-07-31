#pragma once
#include "DirectX11Framework.h"

namespace CommonUtilities
{
	class Timer;
	class InputManager;
}

namespace CU = CommonUtilities;

class MainSingleton
{
public:
	MainSingleton();
	void Create();
	void Destory();

	CU::Timer* GetTimer();
	CU::InputManager* GetInput();
	static MainSingleton* GetInstance();

	void SetWindowHandle(HWND* aHWND);
	HWND* GetWindowHandle();
private:
	CU::InputManager* myInput;
	CU::Timer* myTimer;
	HWND* myHWND;
};

