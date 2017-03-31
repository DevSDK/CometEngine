
#include <Windows.h>
#include <string>
#include "CometEngineWin32.h"

using namespace CometEngine;

extern "C" __declspec(dllexport)  bool __stdcall CometEngineWIn32Init(wchar_t* Title, int Width, int Height, int WindowMode, HINSTANCE hInstance, LPSTR lpCmdLine)
{
	return CometEngineWin32::GetInstance()->InitalizeCometEngine(Title,Width,Height, (WINDOW_MODE)WindowMode, true,4, hInstance, lpCmdLine) ;
}

extern "C" __declspec(dllexport) void LanuchCometEngine()
{
	CometEngineWin32::GetInstance()->Launch();	
}

