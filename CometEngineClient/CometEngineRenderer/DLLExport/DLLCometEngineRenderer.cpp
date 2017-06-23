#include <Windows.h>
#include <string>
#include "../Renderer/CometEngineDXRenderer.h"

using namespace CometEngine::Renderer;

extern "C" __declspec(dllexport)  bool __stdcall Init(int i_RenderWidth, int i_RenderHeight, int i_RenderScale, int i_MSAAScale, bool i_IsFullScreen)
{
	return CometEngineDXRenderer::GetInstance()->Init(i_RenderWidth, i_RenderHeight, i_RenderScale, i_RenderScale, i_IsFullScreen);
}

 extern "C" __declspec(dllexport)  void __stdcall Render(float dt)
{
	CometEngineDXRenderer::GetInstance()->Render(dt);
}

extern "C" __declspec(dllexport)  void __stdcall EnableDebugDevice(bool flag)
{
	CometEngineDXRenderer::GetInstance()->EnableDebugDevice(flag);
}