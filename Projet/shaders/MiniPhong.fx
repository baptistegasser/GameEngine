struct Material
{
    float4 Ambient;
    float4 Roughness;
    float4 Specular;
    float Intensity;
    float3 _FILL_;
};

struct AmbientLight {
    float4 Value;
};

struct DirectionalLight
{
    float4 Direction;
    float4 Specular;
    float4 Roughness;
};

struct PointLight
{
    float3 Position;
    float3 Specular;
    float3 Roughness;
    float InnerRadius, OuterRadius, Intensity;

    float3 CalcPhong(float3 N, float3 V, float3 WorldPosition)
    {
        float3 color = (float4) 0;

        float3 L = Position - WorldPosition;
        L = normalize(L);
        float NdotL = dot(N, L);

        float3 R = reflect(-L, N);

        if (NdotL > 0)
        {
         /*Diffuse color*/
            float dist = distance(WorldPosition, Position);
            float sstep = smoothstep(InnerRadius, OuterRadius, dist);

            float3 diffuseColor = lerp(Specular, float3(0.0f, 0.0f, 0.0f), sstep);
            color += (diffuseColor * NdotL);

         /*Specular color*/
            float RdotV = dot(R, V);
            color += Roughness * pow(max(0.0f, RdotV), Intensity);
        }

        return color;
    }
};

cbuffer param
{ 
	float4x4 MatWorldViewProj;
	float4x4 MatWorld;
	float4 CameraPos;
    AmbientLight Ambient;
    DirectionalLight Directional;
    Material Mat;
	bool HasTexture;
	float3 _FILL_;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3;
};

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

Texture2D textureEntree;
SamplerState SampleState;
StructuredBuffer<PointLight> PointLights;
//StructuredBuffer<BetterSpotLight> SpotLights;

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
	
    // Default add ambiant light
    float3 phong = Ambient.Value;
    
    // TODO directionnal lighting
    
    uint LightCount = 0, Stride;

    // Calc all Point lights
    PointLights.GetDimensions(LightCount, Stride);
    for (uint i = 0; i < LightCount; i += Stride) {
        phong += PointLights[i].CalcPhong(N, V, vs.PosWorld);
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
