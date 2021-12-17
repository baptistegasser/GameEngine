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
	float2 _FILL_;
	float4 PosScale;
	float TextureCoefficient;
    bool IsTunnel;
}

Texture2D texture1;
Texture2D texture2;
Texture2D texture3;
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
VS_Sortie MiniPhongTerrainVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, MatWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), MatWorld).xyz;

	sortie.PosWorld = mul(Pos, MatWorld).xyz;

	sortie.vDirCam = CameraPos.xyz - sortie.PosWorld;

	sortie.coordTex = coordTex;

	return sortie;
}


float4 MiniPhongTerrainPS(VS_Sortie vs) : SV_Target
{
	float3 couleur;
	float3 couleurGazon;
	float3 couleurRoche;
	float3 couleurChemin;

	float2 coordTexChemin;
	coordTexChemin.x = vs.coordTex.x * TextureCoefficient / PosScale.x / PosScale.z;
	coordTexChemin.y = vs.coordTex.y * TextureCoefficient / PosScale.y / PosScale.w;

	// Normalise inputs
	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);

	// Default add ambient light
	float3 phong = AmbientColor.rgb;
    if (IsTunnel)
        phong /= 2.f;

	// Retrieve lights
        uint LightCount = 0, Stride;
	LightsBuffer.GetDimensions(LightCount, Stride);
	// Calc all Point lights
	for (uint i = 0; i < LightCount; i += 1)
	{
        if (IsTunnel && LightsBuffer[i].Type == LightType_Directional)
            continue;
		else
            phong += CalcPhong(N, V, vs.PosWorld, Mat, LightsBuffer[i]);
    }

	// Échantillonner la couleur du pixel à partir de la texture
	float3 colorTex1 = texture1.Sample(SampleState, vs.coordTex).rgb;
	float3 colorTex2 = texture2.Sample(SampleState, vs.coordTex).rgb;
	float3 colorTex3 = texture3.Sample(SampleState, coordTexChemin).rgb;

	// I = A + D * N.L + (R.V)n
	couleurGazon = colorTex1 * saturate(float4(phong, 0.0f));
	couleurRoche = colorTex2 * saturate(float4(phong, 0.0f));
	couleurChemin = colorTex3 * saturate(float4(phong, 0.0f));

	couleur = colorTex3 * couleurGazon + (1 - colorTex3) * couleurRoche;

	return float4(couleur, 1.0f);
}

technique11 MiniPhongTerrain
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MiniPhongTerrainVS()));
		SetPixelShader(CompileShader(ps_5_0, MiniPhongTerrainPS()));
		SetGeometryShader(NULL);
	}
}
