
#include <Windows.h>
#include <string>
#include "../Win32/CometEngineWin32.h"

using namespace CometEngine;

extern "C" __declspec(dllexport)  bool __stdcall Init(wchar_t* Title, int Width, int Height, int WindowMode, HINSTANCE hInstance, LPSTR lpCmdLine)
{
	return CometEngineWin32::GetInstance()->Init(Title,Width,Height, (WINDOW_MODE)WindowMode, hInstance, lpCmdLine) ;
}

extern "C" __declspec(dllexport)  void __stdcall Launch()
{
	CometEngineWin32::GetInstance()->Launch();	
}

