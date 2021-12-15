#include "stdafx.h"
#include "Shader.h"

#include "resources/resource.h"
#include "util/Util.h"
#include "EngineD3D11.h"
#include "Light.h"
#include "Vertex.h"
#include "ShaderTerrain.h"

ShaderTerrain::ShaderTerrain(const wchar_t* FileName)
	: Shader{FileName}
{
	ID3D11Device* PD3DDevice = EngineD3D11::GetInstance().Device->D3DDevice;

	D3D11_BUFFER_DESC BuffDesc;
	ZeroMemory(&BuffDesc, sizeof(BuffDesc));

	BuffDesc.Usage = D3D11_USAGE_DEFAULT;
	BuffDesc.ByteWidth = sizeof(ShadersParamsTerrain);
	BuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BuffDesc.CPUAccessFlags = 0;
	PD3DDevice->CreateBuffer(&BuffDesc, nullptr, &PConstantBuffer);
}
