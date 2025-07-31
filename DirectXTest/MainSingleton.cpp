#include "pch.h"
#include "MainSingleton.h"
#include "InputManager.h"
#include "Timer.h"

static MainSingleton* myInstance = nullptr;

void MainSingleton::Create()
{
	myInstance = this;
	myTimer = new CU::Timer();
	myInput = new CU::InputManager();
}

void MainSingleton::Destory()
{
	delete myTimer;
	myTimer = nullptr;

	delete myInput;
	myInput = nullptr;

	delete myInstance;
	myInstance = nullptr;
}

CU::Timer* MainSingleton::GetTimer()
{
	return myTimer;
}

CU::InputManager* MainSingleton::GetInput()
{
	return myInput;
}

MainSingleton* MainSingleton::GetInstance()
{
	return myInstance;
}

void MainSingleton::SetWindowHandle(HWND* aHWND)
{
	myHWND = aHWND;
}

HWND* MainSingleton::GetWindowHandle()
{
	return myHWND;
}

MainSingleton::MainSingleton()
{
	myInstance = this;
}
