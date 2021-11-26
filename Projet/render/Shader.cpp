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

	const int MAX_LIGHT = 256;

	// Create buffers for light
	D3D11_BUFFER_DESC PointLightsBDesc;
	PointLightsBDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	PointLightsBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PointLightsBDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	PointLightsBDesc.StructureByteStride = sizeof(PointLight);
	PointLightsBDesc.ByteWidth = PointLightsBDesc.StructureByteStride * MAX_LIGHT;
	PointLightsBDesc.Usage = D3D11_USAGE_DYNAMIC;
	PD3DDevice->CreateBuffer(&PointLightsBDesc, nullptr, &PPointLightsBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC PointLightsBVDesc;
	PointLightsBVDesc.Format = DXGI_FORMAT_UNKNOWN;
	PointLightsBVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	PointLightsBVDesc.Buffer.ElementOffset = 0;
	PointLightsBVDesc.Buffer.ElementWidth = MAX_LIGHT;
	PD3DDevice->CreateShaderResourceView(PPointLightsBuffer, &PointLightsBVDesc, &PPointLightsBufferView);
}

Shader::~Shader()
{
	PM3D::DXRelacher(PConstantBuffer);
	PM3D::DXRelacher(PEffectPass);
	PM3D::DXRelacher(PEffectTechnique);
	PM3D::DXRelacher(PInputLayout);
	PM3D::DXRelacher(PEffect);
}

void Shader::UpdateLightsBuffer(ID3D11DeviceContext* PDeviceContext, const LightConfig& LightConfig) const
{
	const auto& PointLights = LightConfig.GetPointLights();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//  Disable GPU access to the vertex buffer data.
	PDeviceContext->Map(PPointLightsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	memcpy(mappedResource.pData, PointLights.data(), sizeof(PointLight) * PointLights.size());
	//  Reenable GPU access to the vertex buffer data.
	PDeviceContext->Unmap(PPointLightsBuffer, 0);

	const auto PointLightsRess = PEffect->GetVariableByName("PointLights")->AsShaderResource();
	PointLightsRess->SetResource(PPointLightsBufferView);
}
