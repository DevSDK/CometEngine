#pragma once
#include <Windows.h>
#include <string>
#include <assert.h>


namespace CometEngine
{

	enum WINDOW_MODE
	{
		DEFAULT = 0,
		NO_BORDER,
		FULLSCREEN
	};


	class CometEngineWin32
	{
	typedef void(*Render_func) (float dt);

	private:
		static CometEngineWin32* Instance;
		static HRESULT CALLBACK MessageProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	public:
		static CometEngineWin32* GetInstance();
		void Launch();

		bool Init(wchar_t * WindowTitle, int WIdth, int Height, WINDOW_MODE WindowMode, HINSTANCE hInstance, LPSTR lpCmdLine);
	private:
		bool InitializeClientWin32(LPSTR lpCmdLine);
		bool LoadRenderer();

		void Update(float delta);

		wchar_t*	mWindowTitle;
		HWND		mHwnd;
		HINSTANCE	mhInstance;
		UINT		mWidth;
		UINT		mHeight;
	
		WINDOW_MODE				mWindowMode;
		Render_func Render;

		~CometEngineWin32();
		CometEngineWin32();
	};
		
	
}