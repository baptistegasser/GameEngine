#pragma once

#include "d3dx11effect.h"

struct ShadersParams {
	DirectX::XMMATRIX matWorldViewProj;
	DirectX::XMMATRIX matWorld;
	DirectX::XMVECTOR vLumiere;
	DirectX::XMVECTOR vCamera;
	DirectX::XMVECTOR vAEcl;
	DirectX::XMVECTOR vAMat;
	DirectX::XMVECTOR vDEcl;
	DirectX::XMVECTOR vDMat;
	DirectX::XMVECTOR vSEcl;
	DirectX::XMVECTOR vSMat;
	float puissance;
	int bTex;
	DirectX::XMFLOAT2 remplissage;

};

struct Shader {
	const wchar_t* FileName;
	ID3D11Buffer* PConstantBuffer;
	
	ID3DX11Effect* PEffect;
	ID3DX11EffectTechnique* PEffectTechnique;
	ID3DX11EffectPass* PEffectPass;
	ID3D11InputLayout* PInputLayout;
	ID3D11SamplerState* PSampleState;

	Shader(const wchar_t* FileName);
	~Shader();
};
