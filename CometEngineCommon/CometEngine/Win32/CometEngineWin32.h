#pragma once
#include <Windows.h>
#include <d3d.h>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <DirectXMath.h>
#include "..\Core\CometEngine\__Debug.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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
	private:
		static CometEngineWin32* Instance;
		static HRESULT CALLBACK MessageProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	public:
		static CometEngineWin32* GetInstance();
		void Launch();

		bool InitalizeCometEngine(wchar_t * WindowTitle, int WIdth, int Height, WINDOW_MODE WindowMode, bool isMSAA, int MsaaLevel, HINSTANCE hInstance, LPSTR lpCmdLine);
	private:
		bool InitializeClientWin32(LPSTR lpCmdLine);
		bool InitializeDirectX();

		void Render(float delta);
		void Update(float delta);

		wchar_t*	mWindowTitle;
		HWND		mHwnd;
		HINSTANCE	mhInstance;
		UINT		mWidth;
		UINT		mHeight;
		bool		mIsMinimized;
		bool		mIsMSAA;
		int			mMsaaLevel;

		WINDOW_MODE				mWindowMode;
		ID3D11Device*			mDXDevice;
		ID3D11DeviceContext*	mDXContext;
		IDXGISwapChain*			mSwapChain;
		ID3D11RenderTargetView*	mRenderTargetView;
		ID3D11DepthStencilView*	mDepthStencilView;
		ID3D11Texture2D*		mDepthStencilBuffer;

		~CometEngineWin32();
		CometEngineWin32();
	};
}