#include "Util.hlsl"
#include "Lights.hlsl"
#include "Material.hlsl"

/****************************************************************************
 *                          Input data and buffers                          *
 ****************************************************************************/
cbuffer param
{ 
	float4x4 MatWorldViewProj;
    float4x4 LightMatWorldViewProj;
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

#define SHADOW_TEX_SIZE 512
Texture2D ShadowTexture;
SamplerState ShadowMapSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3;
    float4 ShadowPos : TEXCOORD4;
};

//////////////////////////////// SHADOW THIS CASUAL

float CalcShadowValue(float4 ShadowPos)
{
    float2 ShadowCoord = 0.5 * ShadowPos.xy / ShadowPos.w + float2(0.5, 0.5);
    ShadowCoord.y = 1.0f - ShadowCoord.y;
    float2 PixelActuel = SHADOW_TEX_SIZE * ShadowCoord;
    float2 lerps = frac(PixelActuel);
    float Profondeur = ShadowPos.z / ShadowPos.w;
    float3 kernel[9];
    float echelle = 1.0 / SHADOW_TEX_SIZE;
    float2 coord[9];
    coord[0] = ShadowCoord + float2(-echelle, -echelle);
    coord[1] = ShadowCoord + float2(-echelle, 0.0);
    coord[2] = ShadowCoord + float2(-echelle, echelle);
    coord[3] = ShadowCoord + float2(0.0, -echelle);
    coord[4] = ShadowCoord + float2(0.0, 0.0);
    coord[5] = ShadowCoord + float2(0.0, echelle);
    coord[6] = ShadowCoord + float2(echelle, -echelle);
    coord[7] = ShadowCoord + float2(echelle, 0.0);
    coord[8] = ShadowCoord + float2(echelle, echelle);
    kernel[0].x = (ShadowTexture.Sample(ShadowMapSampler, coord[0]) < Profondeur) ? 0.0f : 1.0f;
    kernel[0].y = (ShadowTexture.Sample(ShadowMapSampler, coord[1]) < Profondeur) ? 0.0f : 1.0f;
    kernel[0].z = (ShadowTexture.Sample(ShadowMapSampler, coord[2]) < Profondeur) ? 0.0f : 1.0f;
    kernel[1].x = (ShadowTexture.Sample(ShadowMapSampler, coord[3]) < Profondeur) ? 0.0f : 1.0f;
    kernel[1].y = (ShadowTexture.Sample(ShadowMapSampler, coord[4]) < Profondeur) ? 0.0f : 1.0f;
    kernel[1].z = (ShadowTexture.Sample(ShadowMapSampler, coord[5]) < Profondeur) ? 0.0f : 1.0f;
    kernel[2].x = (ShadowTexture.Sample(ShadowMapSampler, coord[6]) < Profondeur) ? 0.0f : 1.0f;
    kernel[2].y = (ShadowTexture.Sample(ShadowMapSampler, coord[7]) < Profondeur) ? 0.0f : 1.0f;
    kernel[2].z = (ShadowTexture.Sample(ShadowMapSampler, coord[8]) < Profondeur) ? 0.0f : 1.0f;
    float3 col12 = lerp(kernel[0], kernel[1], lerps.x);
    float3 col23 = lerp(kernel[1], kernel[2], lerps.x);
    float4 lc;
    lc.x = lerp(col12.x, col12.y, lerps.y);
    lc.y = lerp(col12.y, col12.z, lerps.y);
    lc.z = lerp(col23.x, col23.y, lerps.y);
    lc.w = lerp(col23.y, col23.z, lerps.y);
    return (lc.x + lc.y + lc.z + lc.w) / 4.0;
}



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
    
    sortie.ShadowPos = mul(Pos, LightMatWorldViewProj);

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
    
    float ValeurOmbre = CalcShadowValue(vs.ShadowPos);
    
    // Retrieve lights
    uint LightCount = 0, Stride;
    LightsBuffer.GetDimensions(LightCount, Stride);
    // Calc all Point lights
    for (uint i = 0; i < LightCount; i += 1)
    {
        phong += CalcPhong(N, V, vs.PosWorld, Mat, LightsBuffer[i]);
    }

    return color * saturate(float4(phong, 0.0f)) * ValeurOmbre;
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


struct ShadowMapVS_SORTIE
{
    float4 Pos : SV_POSITION;
    float3 Profondeur : TEXCOORD0;
};

RasterizerState rsCullFront
{
    CullMode = Front;
};
ShadowMapVS_SORTIE ShadowMapVS(float4 Pos : POSITION)
{
    ShadowMapVS_SORTIE Out = (ShadowMapVS_SORTIE) 0;
    // Calcul des coordonnées
    //Out.Pos = mul(Pos, LightMatWorldViewProj); // WVP de la lumiere
    // Obtenir la profondeur et normaliser avec w
    //Out.Profondeur.x = Out.Pos.z / Out.Pos.w;
    return Out;
}

technique11 ShadowMap
{
    pass p0
    {
        VertexShader = compile vs_5_0 ShadowMapVS();
        SetRasterizerState(rsCullFront);
        SetPixelShader(NULL);
        SetGeometryShader(NULL);
    }
}