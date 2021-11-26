#include "stdafx.h"
#include "Shader.h"

#include "resources/resource.h"
#include "util/util.h"
#include "MoteurWindows.h"
#include "Vertex.h"

Shader::Shader(const wchar_t* FileName)
{
	ID3D11Device* PD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	// Cr�ation d'un tampon pour les constantes du VS
	D3D11_BUFFER_DESC BuffDesc;
	ZeroMemory(&BuffDesc, sizeof(BuffDesc));

	BuffDesc.Usage = D3D11_USAGE_DEFAULT;
	BuffDesc.ByteWidth = sizeof(ShadersParams);
	BuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BuffDesc.CPUAccessFlags = 0;
	PD3DDevice->CreateBuffer(&BuffDesc, nullptr, &PConstantBuffer);

	// Pour l'effet
	ID3DBlob* PFXBlob = nullptr;

	DX_TRY(D3DCompileFromFile(FileName, 0, 0, 0, "fx_5_0", 0, 0, &PFXBlob, 0), DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(PFXBlob->GetBufferPointer(), PFXBlob->GetBufferSize(), 0, PD3DDevice, &PEffect);

	PFXBlob->Release();

	PEffectTechnique = PEffect->GetTechniqueByIndex(0);
	PEffectPass = PEffectTechnique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC PassDesc;
	PEffectPass->GetVertexShaderDesc(&PassDesc);

	D3DX11_EFFECT_SHADER_DESC EffectDesc;
	PassDesc.pShaderVariable->GetShaderDesc(PassDesc.ShaderIndex, &EffectDesc);

	DX_TRY(PD3DDevice->CreateInputLayout(Vertex::Layout,
		Vertex::LayoutCount,
		EffectDesc.pBytecode,
		EffectDesc.BytecodeLength,
		&PInputLayout),
		DXE_CREATIONLAYOUT);

	// Initialisation des param�tres de sampling de la texture
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Cr�ation de l'�tat de sampling
	PD3DDevice->CreateSamplerState(&samplerDesc, &PSampleState);
}

Shader::~Shader()
{
	DX_RELEASE(PConstantBuffer);
	DX_RELEASE(PEffectPass);
	DX_RELEASE(PEffectTechnique);
	DX_RELEASE(PInputLayout);
	DX_RELEASE(PSampleState);
	DX_RELEASE(PEffect);
}
