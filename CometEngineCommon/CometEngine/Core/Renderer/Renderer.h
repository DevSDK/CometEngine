#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "..\CometEngine\__Debug.h"

namespace CometEngine
{
	class Renderer
	{
	private:
		ID3D11Device*			mDXDevice;
		ID3D11DeviceContext*	mDXContext;
		IDXGISwapChain*			mSwapChain;
		ID3D11RenderTargetView*	mRenderTargetView;
		ID3D11DepthStencilView*	mDepthStencilView;
		ID3D11Texture2D*		mDepthStencilBuffer;

	};
}