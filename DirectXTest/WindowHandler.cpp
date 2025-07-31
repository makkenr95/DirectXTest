#include "pch.h"
#include "WindowHandler.h"
#include "InputManager.h"
#include "MainSingleton.h"

CWindowHandler::CWindowHandler()
{
	myWindowHandle = nullptr;
}

CWindowHandler::~CWindowHandler()
{
	myWindowHandle = nullptr;
}

LONG CWindowHandler::GetWidth() const
{
	return myWindowData.myWidth;
}

LONG CWindowHandler::GetHeight() const
{
	return myWindowData.myHeight;
}

LRESULT CALLBACK CWindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static CWindowHandler* windowHandler = nullptr;

	if (MainSingleton::GetInstance()->GetInput()->UpdateEvents(uMsg, wParam, lParam))
	{
		return 0;
	}
	
	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_CREATE)
	{
		CREATESTRUCT* createstruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		windowHandler = reinterpret_cast<CWindowHandler*>(createstruct->lpCreateParams);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CWindowHandler::Init(SWindowData aWindowData)
{
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_APPSTARTING);
	windowClass.lpszClassName = L"TEST";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(L"TEST", L"Hoppas detta funkar...",
								WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE, 
								aWindowData.myX, aWindowData.myY, aWindowData.myWidth, aWindowData.myHeight, 
								nullptr, nullptr, nullptr, this);
	myWindowData = aWindowData;
	return true;
}

HWND CWindowHandler::GetWindowHandle()
{
	return myWindowHandle;
}
