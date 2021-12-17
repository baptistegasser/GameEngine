#pragma once

#include "Device.h"

/// <summary>
/// DirectX render device
/// </summary>
class DeviceD3D11 final : public Device
{
public:
	static D3D_FEATURE_LEVEL FeatureLevels[];
	static UINT FeatureLevelCount;

	DeviceD3D11(const CDS_MODE CDSMode, const HWND HWND);
	~DeviceD3D11() override;

	void PresentSpecific() override;

	void ActivateAlphaBlending() const;
	void DeactivateAlphaBlending() const;

	void ActivateCullBack() const;
	void DeactivateCullBack() const;

	void ActivateZBuffer() const;
	void DeactivateZBuffer() const;

	void SetFullScreen() noexcept;
	void SetWindowed() noexcept;

	void ClearView();
	void SetRenderTargetView(ID3D11RenderTargetView *PRenderTargetView, ID3D11DepthStencilView *PDepthStencilView);

	ID3D11Device* D3DDevice{};
	ID3D11DeviceContext* ImmediateContext{};
	IDXGISwapChain* SwapChain{};
	ID3D11RenderTargetView* RenderTargetView{};
	ID3D11Texture2D* DepthTexture{};
	ID3D11DepthStencilView* DepthStencilView{};

	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

	ID3D11DepthStencilState* pDepthStencilDephtEnable;
	ID3D11DepthStencilState* pDepthStencilDephtDisable;

private:
	void InitDepthBuffer();
	void InitBlendStates();

	ID3D11BlendState* AlphaBlendEnable{};
	ID3D11BlendState* AlphaBlendDisable{};

	ID3D11RasterizerState* SolidCullBackRS{};
	ID3D11RasterizerState* SolidNoneCullBackRS{};


};
