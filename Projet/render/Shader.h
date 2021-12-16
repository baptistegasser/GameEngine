#pragma once

#include "d3dx11effect.h"

#include "Effect.h"
#include "util/Util.h"

struct ShaderMaterial
{
	DirectX::XMVECTOR Ambient;
	DirectX::XMVECTOR Roughness;
	DirectX::XMVECTOR Specular;
	float Intensity;

private:
	DX_HLSL_FILL(3);
};

struct ShadersParams {
	DirectX::XMMATRIX MatWorldViewProj;
	DirectX::XMMATRIX LightMatWorldViewProj;
	DirectX::XMMATRIX MatWorld;
	DirectX::XMVECTOR CameraPos;
	DirectX::XMVECTOR AmbientColor;
	ShaderMaterial Mat;
	bool HasTexture;

private:
	DX_HLSL_FILL(3);
};

DX_HLSL_ASSERT_ALLIGN(ShadersParams);

struct Shader {
	const wchar_t* FileName;
	ID3D11Buffer* PConstantBuffer;
	
	ID3DX11Effect* PEffect;
	ID3DX11EffectTechnique* PEffectTechnique;
	ID3DX11EffectPass* PEffectPass;
	ID3D11InputLayout* PInputLayout;
	ID3D11SamplerState* PSampleState;

	ID3D11ShaderResourceView* pTextureD3D;
	ID3D11SamplerState* pSampleState;

	// Light buffers
	ID3D11Buffer* PPointLightsBuffer;
	ID3D11ShaderResourceView* PPointLightsBufferView;

	Shader(const wchar_t* FileName);
	~Shader();

	/// <summary>
	/// Update the lights buffer of the shader.
	/// </summary>
	void UpdateLightsBuffer() const;

	static const int SHADOWMAP_DIM = 512;
	/* Depth texture */
	ID3D11InputLayout* PVertexLayoutShadow;
	ID3D11Texture2D* PDepthTexture;
	ID3D11DepthStencilView* PDepthStencilView;
	ID3D11ShaderResourceView* PDepthShaderResourceView;

private:
	void InitDepthBuffer();
};
