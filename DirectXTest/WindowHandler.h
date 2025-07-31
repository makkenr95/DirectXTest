#pragma once
#include "pch.h"

class CWindowHandler
{
public:
	struct SWindowData
	{
		int myX;
		int myY;
		int myWidth;
		int myHeight;
	};

	CWindowHandler();
	~CWindowHandler();
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	bool Init(SWindowData aWindowData);
	HWND GetWindowHandle();
	LONG GetWidth() const;
	LONG GetHeight() const;
private:
	HWND myWindowHandle;
	SWindowData myWindowData;
};
