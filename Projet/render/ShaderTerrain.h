#pragma once

#include "d3dx11effect.h"
#include "util/Util.h"
#include "render/Shader.h"

/// <summary>
/// Parameters of the terrain's shader
/// </summary>
struct ShadersParamsTerrain : ShadersParams {
	DirectX::XMVECTOR PosScale;
	float TextureCoefficient;
	bool IsTunnel = false;
private:
	DX_HLSL_FILL(2);
};

DX_HLSL_ASSERT_ALLIGN(ShadersParamsTerrain);

/// <summary>
/// Shader for terrains
/// </summary>
struct ShaderTerrain : Shader {
	ShaderTerrain(const wchar_t* FileName);
};
