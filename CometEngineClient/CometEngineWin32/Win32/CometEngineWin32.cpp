#define UNICODE
#include "CometEngineWin32.h"
#include <string>
using namespace CometEngine;
CometEngineWin32* CometEngineWin32::Instance = NULL;

CometEngineWin32::CometEngineWin32()
{

}

CometEngineWin32::~CometEngineWin32()
{

	delete Instance;
}

CometEngineWin32* CometEngineWin32::GetInstance()
{
	if (Instance == NULL)
		Instance = new CometEngineWin32();
	return Instance;
}

HRESULT CALLBACK CometEngineWin32::MessageProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return (HRESULT)DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	CometEngineWin32::GetInstance()->Render(0);
	return 0;
}



void CometEngineWin32::Update(float delta)
{

}

bool CometEngineWin32::Init(wchar_t * WindowTitle, int Width, int Height, WINDOW_MODE WindowMode, HINSTANCE hInstance, LPSTR lpCmdLine)
{
	mWindowTitle = WindowTitle;
	mWidth		 = Width;
	mHeight		 = Height;
	mWindowMode  = WindowMode;
	mhInstance	 = hInstance;
	return InitializeClientWin32(lpCmdLine) && LoadRenderer();
}
bool CometEngineWin32::InitializeClientWin32(LPSTR lpCmdLine)
{
	WNDCLASSEX wndx = { 0 };
	wndx.cbClsExtra = 0;
	wndx.cbWndExtra = 0;
	wndx.cbSize = sizeof(WNDCLASSEX);
	wndx.style = CS_HREDRAW | CS_VREDRAW;
	wndx.hInstance = mhInstance;
	wndx.lpfnWndProc = (WNDPROC)MessageProc;
	wndx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndx.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wndx.lpszMenuName = NULL;
	wndx.lpszClassName = L"CometEngineApplication";

	if (!RegisterClassEx(&wndx))
	{
		OutputDebugString(L"\n Create Window FAILED");
		OutputDebugString(std::to_wstring(GetLastError()).c_str());

		return false;
	}

	 mHwnd = CreateWindow(L"CometEngineApplication", mWindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight,
		NULL, (HMENU)NULL, mhInstance, NULL);

	if (mWindowMode == WINDOW_MODE::NO_BORDER)
	{
		SetWindowLong(mHwnd, GWL_STYLE, 0);
	}

	ShowWindow(mHwnd, SW_SHOW);

	return true;

	
}

void CometEngineWin32::Launch()
{
	MSG msg = { 0, };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(0);

		}
	}
	PostQuitMessage(0);
}

bool CometEngineWin32::LoadRenderer()
{
 	HINSTANCE renderer = LoadLibrary(L"CometEngineDXRenderer.dll");
	
	if (renderer == NULL)
		return false;
	Render = (Render_func)GetProcAddress(renderer, "Render");
	if (Render == NULL)
		return false;
//	if (__DEBUG__) 
		
	
	return true;
}	