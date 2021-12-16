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
    int HasTexture;
    int EnableDirLight;
	float2 _FILL_;
}

Texture2D textureEntree;
SamplerState SampleState;
StructuredBuffer<Light> LightsBuffer;

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3;
};


/****************************************************************************
 *                               S H A D E R S !                            *
 ****************************************************************************/
VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, MatWorldViewProj);
    sortie.Norm = mul(float4(Normale, 0.0f), MatWorld).xyz;

    sortie.PosWorld = mul(Pos, MatWorld).xyz;

    sortie.vDirCam = CameraPos.xyz - sortie.PosWorld;

	sortie.coordTex = coordTex;

	return sortie;
}

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
   // Default color : missing magenta
    float4 color = float4(1.f, 0.f, 1.f, 1.f);

   // Use texture if any
    if (HasTexture > 0) {
        color = textureEntree.Sample(SampleState, vs.coordTex);
    }

	// Normalise inputs
	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);
	
    // Default add ambient light
    float3 phong = AmbientColor.rgb;
    
    // Retrieve lights
    uint LightCount = 0, Stride;
    LightsBuffer.GetDimensions(LightCount, Stride);
    // Calc all Point lights
    for (uint i = 0; i < LightCount; i += 1)
    {
        if (LightsBuffer[i].Type == LightType_Directional && !EnableDirLight)
            continue;
        
        phong += CalcPhong(N, V, vs.PosWorld, Mat, LightsBuffer[i]);
    }

    return color * saturate(float4(phong, 0.0f));
}

technique11 MiniPhong
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MiniPhongVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongPS()));
		SetGeometryShader(NULL);
	}
}
