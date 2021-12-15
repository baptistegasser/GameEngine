#pragma once

#include "d3dx11effect.h"
#include "util/Util.h"
#include "render/Shader.h"

struct ShadersParamsTerrain : ShadersParams {
	DirectX::XMVECTOR PosScale;
	float TextureCoefficient;
private:
	DX_HLSL_FILL(3);
};

DX_HLSL_ASSERT_ALLIGN(ShadersParamsTerrain);

struct ShaderTerrain : Shader {
	ShaderTerrain(const wchar_t* FileName);
};
