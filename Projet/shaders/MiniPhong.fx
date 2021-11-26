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
        float3 color = (float3) 1.0;

        float3 L = normalize(Position - WorldPosition);
        
        // Valeur de la composante diffuse 
        float3 diff = saturate(dot(N, L));
 
         // R = 2 * (N.L) * N – L 
        float3 R = normalize(2 * diff * N - L);
 
         // Puissance de 4 - pour l’exemple 
        float S = pow(saturate(dot(R, V)), 4.0f);
 
         // Échantillonner la couleur du pixel à partir de la texture 
        //float3 couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;
 
         // I = A + D * N.L + (R.V)n 
        color = color * Roughness.rgb * diff;
 
        color += S;
  
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
    float3 phong = Ambient.Value.rgb;
    
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
