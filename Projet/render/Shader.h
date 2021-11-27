#pragma once

#include "d3dx11effect.h"

#include "LightConfig.h"

struct ShaderMaterial
{
	DirectX::XMVECTOR Ambient;
	DirectX::XMVECTOR Roughness;
	DirectX::XMVECTOR Specular;
	float Intensity;
	DirectX::XMFLOAT3 _FILL_;
};

struct ShadersParams {
	DirectX::XMMATRIX MatWorldViewProj;
	DirectX::XMMATRIX MatWorld;
	DirectX::XMVECTOR CameraPos;
	AmbientLight Ambient;
	DirectionalLight Directional;
	ShaderMaterial Mat;
	bool HasTexture;
	DirectX::XMFLOAT3 _FILL_;
};
static_assert(sizeof(ShadersParams) % 16 == 0);

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
	ID3D11Buffer* PSpotLightsBuffer;
	ID3D11ShaderResourceView* PSpotLightsBufferView;

	Shader(const wchar_t* FileName);
	~Shader();

	void UpdateLightsBuffer(ID3D11DeviceContext* PDeviceContext, const LightConfig& LightConfig) const;
};
