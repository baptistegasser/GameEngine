#pragma once

#include "math/Vec3f.h"
#include "DeviceD3D11.h"

#include <d3d11.h>
#include "d3dx11effect.h"

struct EffectVertex {
	EffectVertex() = default;
	EffectVertex(const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT2& CoordTex)
		: Position{ Position }
		, CoordTex{ CoordTex }
	{}

	static UINT LayoutSize;
	static D3D11_INPUT_ELEMENT_DESC Layout[];
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 CoordTex;
};

class Effect {
public:
	const wchar_t* FileName;

	explicit Effect(const wchar_t* FileName);
	~Effect() noexcept;
	virtual void Draw();
	void DebutPostEffect();
	void FinPostEffect();

private:
	static const EffectVertex Vertices[6];
	ID3D11Buffer* PVertexBuffer;
	DeviceD3D11* PDevice;

	ID3DX11Effect* PEffet;
	ID3DX11EffectTechnique* PTechnique;
	ID3DX11EffectPass* PPass;
	ID3D11InputLayout* PVertexLayout;
	ID3D11SamplerState* PSampleState;

	ID3D11Texture2D* PTextureScene;
	ID3D11RenderTargetView* PRenderTargetView;
	ID3D11ShaderResourceView* PResourceView;
	ID3D11Texture2D* PDepthTexture;
	ID3D11DepthStencilView* PDepthStencilView;

	ID3D11RenderTargetView* POldRenderTargetView;
	ID3D11DepthStencilView* POldDepthStencilView;
};