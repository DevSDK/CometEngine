#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace CometEngine
{
	namespace Renderer
	{

		class CometEngineDXRenderer
		{
		public:
			static CometEngineDXRenderer* GetInstance();
			void EnableDebugDevice(bool i_Flag);
			bool Init(int i_RenderWidth, int i_RenderHeight, int i_RenderScale, int i_MSAAScale, bool isFullScreen);
			//TODO: Must Be Render Geometry (Maybe Implementing Render List)
			void Render(float dt);

		private:
			static CometEngineDXRenderer* Instance;

			CometEngineDXRenderer() {}
			~CometEngineDXRenderer();

			BOOL		m_IsMSAA;
			BOOL		m_IsDebugFlag;
			BOOL		m_IsFullScreen;

			HWND		m_hRenderTargetHwnd;

			UINT		m_uMsaaLevel;
			UINT		m_uRenderScale;
			UINT		m_uRenderWidth;
			UINT		m_uRenderHeight;

			//TODO: Must Be Relase Resources
			ID3D11Device*			mDXDevice;
			ID3D11DeviceContext*	mDXContext;
			IDXGISwapChain*			mSwapChain;
			ID3D11RenderTargetView*	mRenderTargetView;
			ID3D11DepthStencilView*	mDepthStencilView;
			ID3D11Texture2D*		mDepthStencilBuffer;


		};

	}
}