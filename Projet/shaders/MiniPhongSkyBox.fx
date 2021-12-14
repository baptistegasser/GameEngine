#include "Util.hlsl"
#include "Lights.hlsl"
#include "Material.hlsl"

/****************************************************************************
 *                          Input data and buffers                          *
 ****************************************************************************/
cbuffer param
{ 
	float4x4 MatWorldViewProj;
	float4x4 MatWorld;
	float4 CameraPos;
    float4 AmbientColor;
    Material Mat;
	bool HasTexture;
	float3 _FILL_;
}

Texture2D textureEntree;
SamplerState SampleState;
StructuredBuffer<Light> LightsBuffer;

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0;
};


/****************************************************************************
 *                               S H A D E R S !                            *
 ****************************************************************************/
VS_Sortie MiniPhongSkyBoxVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, MatWorldViewProj);

	sortie.coordTex = coordTex;

	return sortie;
}

float4 MiniPhongSkyBoxPS( VS_Sortie vs ) : SV_Target
{
   // Default color : missing magenta
    float4 color = float4(1.f, 0.f, 1.f, 1.f);

   // Use texture if any
    if (HasTexture > 0) {
        color = textureEntree.Sample(SampleState, vs.coordTex);
    }

    return color;
}

technique11 MiniPhongSkyBox
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MiniPhongSkyBoxVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongSkyBoxPS()));
		SetGeometryShader(NULL);
	}
}
