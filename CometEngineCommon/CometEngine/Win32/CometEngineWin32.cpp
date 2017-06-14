#define UNICODE
#include "CometEngineWin32.h"
#include <string>
#include <DirectXColors.h>
#define HR(x) { HRESULT nValue = (x); if(FAILED(nValue)) return false; } 
using namespace CometEngine;
CometEngineWin32* CometEngineWin32::Instance = NULL;

CometEngineWin32::CometEngineWin32()
{

}

CometEngineWin32::~CometEngineWin32()
{
	mSwapChain->Release();
	mDepthStencilView->Release();
	mDXDevice->Release();
	mDXContext->Release();
	mRenderTargetView->Release();
	mDepthStencilBuffer->Release();
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
	return 0;
}

void CometEngineWin32::Render(float delta)
{
	assert(mDXContext);
	assert(mSwapChain);

	mDXContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::Aqua);
	mDXContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);





	

	mSwapChain->Present(0, 0);
}

void CometEngineWin32::Update(float delta)
{

}

bool CometEngineWin32::InitalizeCometEngine(wchar_t * WindowTitle, int Width, int Height, WINDOW_MODE WindowMode, bool isMSAA, int MsaaLevel, HINSTANCE hInstance, LPSTR lpCmdLine)
{
	mWindowTitle = WindowTitle;
	mWidth		 = Width;
	mHeight		 = Height;
	mWindowMode  = WindowMode;
	mhInstance	 = hInstance;
	mIsMSAA		 = isMSAA;
	mMsaaLevel	 = MsaaLevel;

	return InitializeClientWin32(lpCmdLine) && InitializeDirectX();
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
	mIsMinimized = false;
	 
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
			Render(0);
		}
	}
	PostQuitMessage(0);
}

bool CometEngineWin32::InitializeDirectX()
{

	int flags = 0;
	if (__DEBUG__)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL Featurelevel;
	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, 0, 0, D3D11_SDK_VERSION, &mDXDevice, &Featurelevel, &mDXContext));
	
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {0};
	swap_chain_desc.BufferDesc.Width = mWidth;
	swap_chain_desc.BufferDesc.Height = mHeight;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	UINT MsaaQulity;
	HR(mDXDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMsaaLevel, &MsaaQulity));
	
	assert(MsaaQulity > 0);
	if (mIsMSAA)
	{
		swap_chain_desc.SampleDesc.Count = mMsaaLevel;
		swap_chain_desc.SampleDesc.Quality = MsaaQulity - 1;
	}
	else
	{
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
	}
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = mHwnd;
	swap_chain_desc.Windowed =	mWindowMode != WINDOW_MODE::FULLSCREEN;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DXGI_ADAPTER_DESC desc;

	IDXGIDevice*	dxgiDevice  = nullptr;
	IDXGIAdapter*	dxgiAdapter = nullptr;
	IDXGIFactory*	dxgiFactory = nullptr;
	ID3D11Texture2D* BackBuffer;
	HR(mDXDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	HR(dxgiFactory->CreateSwapChain(mDXDevice, &swap_chain_desc, &mSwapChain))
	HR(mSwapChain->GetBuffer(NULL ,__uuidof(ID3D11Texture2D), (void**)&BackBuffer));
	mDXDevice->CreateRenderTargetView(BackBuffer, 0, &mRenderTargetView);



	//TODO: Must Be Create Class maybe name "CESystemInfo" Get System Information From DXGIAdapter

    D3D11_TEXTURE2D_DESC	DepthStencillDesc = { 0 };
	DepthStencillDesc.Width		= mWidth;
	DepthStencillDesc.Height	= mHeight;
	DepthStencillDesc.MipLevels = 1;
	DepthStencillDesc.ArraySize = 1;
	DepthStencillDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mIsMSAA)
	{
		DepthStencillDesc.SampleDesc.Count = mMsaaLevel;
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
	viewport.Width = mWidth;
	viewport.Height = mHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	mDXContext->RSSetViewports(1, &viewport);

	dxgiAdapter->Release();
	dxgiDevice->Release();
	dxgiFactory->Release();
	
	
	return true;
}	