#include "CometEngineDXRenderer.h"
#include <DirectXColors.h>
#include <string>
using namespace CometEngine;
using namespace Renderer;

#define HR(x) { HRESULT nValue = (x); if(FAILED(nValue)) return false; } 

CometEngineDXRenderer* CometEngineDXRenderer::Instance = nullptr;

CometEngineDXRenderer * CometEngineDXRenderer::GetInstance()
{
	if (Instance == nullptr)
		Instance = new CometEngineDXRenderer();
	return Instance;

}

void CometEngine::Renderer::CometEngineDXRenderer::EnableDebugDevice(bool i_Flag)
{
	m_IsDebugFlag = i_Flag;
}

bool CometEngine::Renderer::CometEngineDXRenderer::Init(int i_RenderWidth, int i_RenderHeight, int i_RenderScale, int i_MSAAScale, bool isFullScreen)
{
	m_uRenderWidth  = i_RenderWidth;
	m_uRenderHeight = i_RenderHeight;
	m_uRenderScale = i_RenderScale;
	m_uMsaaLevel = i_MSAAScale;
	m_IsFullScreen = isFullScreen;

	UINT flags = 0;
	if (m_IsDebugFlag)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL Featurelevel;
	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, 0, 0, D3D11_SDK_VERSION, &mDXDevice, &Featurelevel, &mDXContext));

	DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
	swap_chain_desc.BufferDesc.Width = m_uRenderWidth;
	swap_chain_desc.BufferDesc.Height = m_uRenderHeight;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	UINT MsaaQulity;
	HR(mDXDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_uMsaaLevel, &MsaaQulity));

	assert(MsaaQulity > 0);
	if (m_uMsaaLevel > 1)
	{
		swap_chain_desc.SampleDesc.Count = m_uMsaaLevel;
		swap_chain_desc.SampleDesc.Quality = MsaaQulity - 1;
	}
	else
	{
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
	}
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = m_hRenderTargetHwnd;
	swap_chain_desc.Windowed = m_IsFullScreen;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DXGI_ADAPTER_DESC desc;

	IDXGIDevice*	dxgiDevice = nullptr;
	IDXGIAdapter*	dxgiAdapter = nullptr;
	IDXGIFactory*	dxgiFactory = nullptr;
	ID3D11Texture2D* BackBuffer;
	HR(mDXDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	HR(dxgiFactory->CreateSwapChain(mDXDevice, &swap_chain_desc, &mSwapChain))
	HR(mSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (void**)&BackBuffer));
	HR(mDXDevice->CreateRenderTargetView(BackBuffer, 0, &mRenderTargetView));

	//TODO: Must Be Create Class maybe name "CESystemInfo" Get System Information From DXGIAdapter

	D3D11_TEXTURE2D_DESC	DepthStencillDesc = { 0 };
	DepthStencillDesc.Width = m_uRenderWidth;
	DepthStencillDesc.Height = m_uRenderHeight;
	DepthStencillDesc.MipLevels = 1;
	DepthStencillDesc.ArraySize = 1;
	DepthStencillDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_uMsaaLevel > 1)
	{
		DepthStencillDesc.SampleDesc.Count = m_uMsaaLevel;
		DepthStencillDesc.SampleDesc.Quality = MsaaQulity - 1;
	}
	else
	{
		DepthStencillDesc.SampleDesc.Count = 1;
		DepthStencillDesc.SampleDesc.Quality = 0;
	}

	DepthStencillDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencillDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencillDesc.CPUAccessFlags = 0;
	DepthStencillDesc.MiscFlags = 0;

	HR(mDXDevice->CreateTexture2D(&DepthStencillDesc, 0, &mDepthStencilBuffer));
	HR(mDXDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));
	mDXContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_uRenderWidth;
	viewport.Height = m_uRenderHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	mDXContext->RSSetViewports(1, &viewport);

	dxgiAdapter->Release();
	dxgiDevice->Release();
	dxgiFactory->Release();

	return true;
}
 

void CometEngineDXRenderer::Render(float dt)
{
	assert(mDXContext);
	assert(mSwapChain);

	mDXContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::Aqua);
	mDXContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mSwapChain->Present(0, 0);
}

CometEngine::Renderer::CometEngineDXRenderer::~CometEngineDXRenderer()
{
	mSwapChain->Release();
	mDepthStencilView->Release();
	mDXDevice->Release();
	mDXContext->Release();
	mRenderTargetView->Release();
	mDepthStencilBuffer->Release();
}
