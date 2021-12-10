/****************************************************************************
 *                                  Utils                                   *
 ****************************************************************************/
// Macro used to fill to match 16 byte stride
#define FILL(size) float##size __FILL__
// Used for some default values
#define BLACK float3(0.f, 0.f, 0.f)

/****************************************************************************
 *                     Rendering data structures                            *
 ****************************************************************************/
// Reprensetation of a material
struct Material
{
    float4 Ambient;
    float4 Roughness;
    float4 Specular;
    float Intensity;
    float3 _FILL_;
};

/****************************************************************************
 *                   Light data structures and methods                      *
 ****************************************************************************/
// Light types definition
#define LightType_Spot 0
#define LightType_Point 1
#define LightType_Directionnal 2

// Generic Representation of a light
struct Light
{
    int Type;
    float3 Color;
    float3 Position;
    float3 Direction;
    float Range;
    float SpotAngle;
    float Intensity;

    FILL(3);
};

// Calculate the phong value for a spot light
float3 CalcSpotPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    return BLACK;
}

// Calculate the phong value for a point light
float3 CalcPointPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    float3 color = (float3) 1.0;

    float3 L = light.Position - Pos;
    float distance = length(L);
    L /= distance;
        
    // Valeur de la composante diffuse 
    float3 diff = saturate(dot(N, L));
        
    // specular shading
    float3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(V, reflectDir), 0.0), mat.Ambient);
        
    // attenuation
    // calculate basic attenuation
    float denom = (distance / light.Range) + 1;
    float attenuation = 1 / (denom * denom);
    attenuation = max(attenuation, 0);

    // combine results
    float3 ambient = light.Color * mat.Roughness.xyz;
    float3 diffuse = light.Color * diff * mat.Roughness.xyz;
    float3 specular = light.Color * spec * mat.Specular.xyz;

    return (ambient + diffuse + specular) * attenuation;
}

// Calculate the phong value for a directionnal light
float3 CalcDirectionnalPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    return BLACK;
}

// Compute the phong value given by a light
float3 CalcPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    switch (light.Type)
    {
        case LightType_Spot:
            return CalcSpotPhong(N, V, Pos, mat, light);
        case LightType_Point:
            return CalcPointPhong(N, V, Pos, mat, light);
        case LightType_Directionnal:
            return CalcDirectionnalPhong(N, V, Pos, mat, light);
        default:
            return BLACK;
    }
}

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
	
    // Default add ambiant light
    float3 phong = AmbientColor.rgb;
    
    // Retrieve lights
    uint LightCount = 0, Stride;
    LightsBuffer.GetDimensions(LightCount, Stride);
    // Calc all Point lights
    for (int i = 0; i < LightCount; i += 1)
    {
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
