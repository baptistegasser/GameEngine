#include "stdafx.h"
#include "Shader.h"

#include "resources/resource.h"
#include "util/util.h"
#include "sommetbloc.h"
#include "MoteurWindows.h"

Shader::Shader(const wchar_t* FileName)
{
	/* Creation of constant buffer : cbuffer */
	ID3D11Device* PD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	// Création d'un tampon pour les constantes du VS
	D3D11_BUFFER_DESC BuffDesc;
	ZeroMemory(&BuffDesc, sizeof(BuffDesc));

	BuffDesc.Usage = D3D11_USAGE_DEFAULT;
	BuffDesc.ByteWidth = sizeof(ShadersParams);
	BuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BuffDesc.CPUAccessFlags = 0;
	PD3DDevice->CreateBuffer(&BuffDesc, nullptr, &PConstantBuffer);

	// Pour l'effet
	ID3DBlob* PFXBlob = nullptr;

	PM3D::DXEssayer(D3DCompileFromFile(FileName, 0, 0, 0, "fx_5_0", 0, 0, &PFXBlob, 0), DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(PFXBlob->GetBufferPointer(), PFXBlob->GetBufferSize(), 0, PD3DDevice, &PEffect);

	PFXBlob->Release();

	PEffectTechnique = PEffect->GetTechniqueByIndex(0);
	PEffectPass = PEffectTechnique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC PassDesc;
	PEffectPass->GetVertexShaderDesc(&PassDesc);

	D3DX11_EFFECT_SHADER_DESC EffectDesc;
	PassDesc.pShaderVariable->GetShaderDesc(PassDesc.ShaderIndex, &EffectDesc);

	PM3D::DXEssayer(PD3DDevice->CreateInputLayout(PM3D::CSommetBloc::layout,
		PM3D::CSommetBloc::numElements,
		EffectDesc.pBytecode,
		EffectDesc.BytecodeLength,
		&PInputLayout),
		DXE_CREATIONLAYOUT);

	// Initialisation des paramètres de sampling de la texture
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

	// Création de l'état de sampling
	PD3DDevice->CreateSamplerState(&samplerDesc, &PSampleState);

	int LightCount = 10;

	// Create dynamic buffer for lighting
	D3D11_BUFFER_DESC LightBuffDesc;
	LightBuffDesc.ByteWidth = MAX_LIGHT_BYTE_WIDTH * LightCount;
	LightBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	LightBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	LightBuffDesc.StructureByteStride = MAX_LIGHT_BYTE_WIDTH;
	PD3DDevice->CreateBuffer(&LightBuffDesc, nullptr, &PLightBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = LightCount;
	PD3DDevice->CreateShaderResourceView(m_lightBuffer2, &srvDesc, &m_pSRV);










	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(LightShader);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	PD3DDevice->CreateBuffer(&bufferDesc, nullptr, &PConstantBuffer2);


	// 3e
	D3D11_BUFFER_DESC buffer;
	ZeroMemory(&buffer, sizeof(bufferDesc));

	buffer.Usage = D3D11_USAGE_DYNAMIC;
	buffer.ByteWidth = sizeof(LightShader)*10;
	buffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	buffer.MiscFlags = 0;
	buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer.StructureByteStride = sizeof(LightShader);

	auto result3 = PD3DDevice->CreateBuffer(&buffer, nullptr, &PStructuredBuffer3);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 10;
	srvDesc.Buffer.ElementWidth = sizeof(LightShader);

	auto result2 = PD3DDevice->CreateShaderResourceView(PStructuredBuffer3, &srvDesc, &ShaderResourceView);
	
	int b =0;
}

Shader::~Shader()
{
	PM3D::DXRelacher(PConstantBuffer);
	PM3D::DXRelacher(PEffectPass);
	PM3D::DXRelacher(PEffectTechnique);
	PM3D::DXRelacher(PInputLayout);
	PM3D::DXRelacher(PEffect);
}
