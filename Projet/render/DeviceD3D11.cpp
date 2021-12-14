#include "stdafx.h"
#include "resources/resource.h"
#include "DeviceD3D11.h"
#include "util/Util.h"
#include "DeviceInfoD3D11.h"

D3D_FEATURE_LEVEL DeviceD3D11::FeatureLevels[] =
{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
};

UINT DeviceD3D11::FeatureLevelCount = ARRAYSIZE(FeatureLevels);

DeviceD3D11::DeviceD3D11(const CDS_MODE CDSMode, const HWND HWND)
{
	UINT CreateDeviceFlags = 0;

#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC SwapDesc;
	SwapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Permettre l’échange plein écran
	ZeroMemory(&SwapDesc, sizeof(SwapDesc));

	switch (CDSMode)
	{
	case CDS_MODE::CDS_FENETRE:
		SwapDesc.Windowed = TRUE;
		break;
	case CDS_MODE::CDS_PLEIN_ECRAN:
		SwapDesc.Windowed = FALSE;
		break;
	}

	DXGI_MODE_DESC Desc;
	Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.Height = 1080;
	Desc.Width = 1920;
	Desc.RefreshRate.Numerator = 60;
	Desc.RefreshRate.Denominator = 1;
	Desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	Desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DeviceInfoD3D11 Device(Desc);
	Desc = Device.Mode;
	ScreenWidth = Desc.Width;
	ScreenHeight = Desc.Height;

	SwapDesc.BufferCount = 1;
	SwapDesc.BufferDesc.Width = Desc.Width;
	SwapDesc.BufferDesc.Height = Desc.Height;
	SwapDesc.BufferDesc.Format = Desc.Format;
	SwapDesc.BufferDesc.RefreshRate.Numerator = Desc.RefreshRate.Numerator;
	SwapDesc.BufferDesc.RefreshRate.Denominator = Desc.RefreshRate.Denominator;
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.OutputWindow = HWND;
	SwapDesc.SampleDesc.Count = 1;
	SwapDesc.SampleDesc.Quality = 0;

	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect(HWND, &rcClient);
	GetWindowRect(HWND, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(HWND, rcWindow.left, rcWindow.top, ScreenWidth + ptDiff.x, ScreenHeight + ptDiff.y, TRUE);

	DX_TRY(D3D11CreateDeviceAndSwapChain(
		       nullptr,
		       D3D_DRIVER_TYPE_HARDWARE,
		       nullptr,
		       CreateDeviceFlags,
		       FeatureLevels, FeatureLevelCount,
		       D3D11_SDK_VERSION,
		       &SwapDesc,
		       &SwapChain,
		       &D3DDevice,
		       nullptr,
		       &ImmediateContext),
		       DXE_ERREURCREATIONDEVICE);

	// Create render target view
	ID3D11Texture2D* BackBuffer;
	DX_TRY(SwapChain->GetBuffer(
		       0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&BackBuffer)),
	       DXE_ERREUROBTENTIONBUFFER);
	DX_TRY(D3DDevice->CreateRenderTargetView(
		       BackBuffer,nullptr, &RenderTargetView), DXE_ERREURCREATIONRENDERTARGET);
	BackBuffer->Release();

	InitDepthBuffer();
	InitBlendStates();

	ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	D3D11_VIEWPORT Viewport;
	Viewport.Width = static_cast<FLOAT>(ScreenWidth);
	Viewport.Height = static_cast<FLOAT>(ScreenHeight);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	ImmediateContext->RSSetViewports(1, &Viewport);

	D3D11_RASTERIZER_DESC RsDesc;
	ZeroMemory(&RsDesc, sizeof(D3D11_RASTERIZER_DESC));
	RsDesc.FillMode = D3D11_FILL_SOLID;
	RsDesc.CullMode = D3D11_CULL_BACK;
	RsDesc.FrontCounterClockwise = false;
	D3DDevice->CreateRasterizerState(&RsDesc, &SolidCullBackRS);

	RsDesc.CullMode = D3D11_CULL_NONE;
	D3DDevice->CreateRasterizerState(&RsDesc, &SolidNoneCullBackRS);

	ImmediateContext->RSSetState(SolidCullBackRS);
}

DeviceD3D11::~DeviceD3D11()
{
	// Switch in windowed mode before closing to avoid crash
	SwapChain->SetFullscreenState(FALSE, nullptr);

	DX_RELEASE(AlphaBlendEnable);
	DX_RELEASE(AlphaBlendDisable);
	DX_RELEASE(SolidCullBackRS);
	DX_RELEASE(SolidNoneCullBackRS);
	DX_RELEASE(DepthTexture);
	DX_RELEASE(pDepthStencilDephtEnable);
	DX_RELEASE(pDepthStencilDephtDisable);
	DX_RELEASE(DepthStencilView);
	DX_RELEASE(RenderTargetView);
	DX_RELEASE(SwapChain);
	DX_RELEASE(D3DDevice);

	if (ImmediateContext)
	{
		ImmediateContext->ClearState();
		ImmediateContext->Flush();
	}
	DX_RELEASE(ImmediateContext);
}

void DeviceD3D11::PresentSpecific()
{
	SwapChain->Present(0, 0);
}

void DeviceD3D11::InitDepthBuffer()
{
	D3D11_TEXTURE2D_DESC DepthTextureDesc;
	ZeroMemory(&DepthTextureDesc, sizeof(DepthTextureDesc));
	DepthTextureDesc.Width = ScreenWidth;
	DepthTextureDesc.Height = ScreenHeight;
	DepthTextureDesc.MipLevels = 1;
	DepthTextureDesc.ArraySize = 1;
	DepthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthTextureDesc.SampleDesc.Count = 1;
	DepthTextureDesc.SampleDesc.Quality = 0;
	DepthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthTextureDesc.CPUAccessFlags = 0;
	DepthTextureDesc.MiscFlags = 0;

	DX_TRY(D3DDevice->CreateTexture2D(
		&DepthTextureDesc, nullptr, &DepthTexture), DXE_ERREURCREATIONTEXTURE);

	// Create depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC DescDSView;
	ZeroMemory(&DescDSView, sizeof(DescDSView));
	DescDSView.Format = DepthTextureDesc.Format;
	DescDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DescDSView.Texture2D.MipSlice = 0;

	DX_TRY(D3DDevice->CreateDepthStencilView(
		       DepthTexture, &DescDSView, &DepthStencilView),DXE_ERREURCREATIONDEPTHSTENCILTARGET);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	DX_TRY(D3DDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilDephtEnable), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
	dsDesc.DepthEnable = false;
	DX_TRY(D3DDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilDephtDisable), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
}

void DeviceD3D11::InitBlendStates()
{
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

	// Basic alpha blending settings
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DX_TRY(D3DDevice->CreateBlendState(
		&BlendDesc, &AlphaBlendEnable),DXE_ERREURCREATION_BLENDSTATE);

	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	DX_TRY(D3DDevice->CreateBlendState(
		&BlendDesc, &AlphaBlendDisable),DXE_ERREURCREATION_BLENDSTATE);
}

void DeviceD3D11::ActivateAlphaBlending() const
{
	constexpr float Factor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ImmediateContext->OMSetBlendState(AlphaBlendEnable, Factor, 0xffffffff);
}


void DeviceD3D11::DeactivateAlphaBlending() const
{
	constexpr float Factor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ImmediateContext->OMSetBlendState(AlphaBlendDisable, Factor, 0xffffffff);
}

void DeviceD3D11::ActivateCullBack() const
{
	ImmediateContext->RSSetState(SolidCullBackRS);
}

void DeviceD3D11::DeactivateCullBack() const
{
	ImmediateContext->RSSetState(SolidNoneCullBackRS);
}

void DeviceD3D11::ActivateZBuffer() const
{
	ImmediateContext->OMSetDepthStencilState(pDepthStencilDephtEnable, 0);
}

void DeviceD3D11::DeactivateZBuffer() const
{
	ImmediateContext->OMSetDepthStencilState(pDepthStencilDephtDisable, 0);
}
