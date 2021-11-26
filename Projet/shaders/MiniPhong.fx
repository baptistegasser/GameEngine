//#include "./shaders/Light.hlsli"

//*****************************************//
//       Structures for lights data        //
//*****************************************//

// Interface for ligth, each class must define it's lightning behaviour
interface ILight
{
    float3 IlluminateAmbient(float3 vNormal);
    float3 IlluminateDiffuse(float3 vNormal);
    float3 IlluminateSpecular(float3 vNormal, float specularPower);
};

// Common base for all lights
struct BaseLightAttrib
{
    float3 Color;
    float Intensity;
};

// Ambiant light is just a simple unidirectional light
struct AmbiantLight : BaseLightAttrib, ILight {
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,1.0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, float specularPower) { return float3(0,0,0); }
};

// A light with ray directions
struct DirectionalLight : BaseLightAttrib, ILight {
    float3 Direction;
    float _DataAlign_;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, float specularPower) { return float3(0,0,0); }
};

// A light bulb representation, intensity fade toward 0 as we go further from the position
struct PointLight : BaseLightAttrib, ILight {
    float3 Position;
    float Range;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, float specularPower) { return float3(0,0,0); }
};


// Simmilar to a spotlight but with a restrictive angle, like a flashligth
struct SpotLight : BaseLightAttrib, ILight {
    float3 Position;
    float3 Direction;
    float Angle;
    float Range;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, float specularPower) { return float3(0,0,0); }
};

StructuredBuffer<BaseLightAttrib> Lights;

cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vLumiere; 		// la position de la source d'�clairage (Point)
	float4 vCamera; 			// la position de la cam�ra
	float4 vAEcl;
	float4 vAMat;
	float4 vDEcl;
	float4 vDMat;
	float4 vSEcl;
	float4 vSMat;
	float puissance;
	int bTex;		    // Bool�en pour la pr�sence de texture
	float2 remplissage;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm : TEXCOORD0;
	float3 vDirLum : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
};

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), matWorld).xyz;

	float3 PosWorld = mul(Pos, matWorld).xyz;

	sortie.vDirLum = vLumiere.xyz - PosWorld;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	// Coordonn�es d'application de texture
	sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'�tat de sampling

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	float3 couleur;

	// Normaliser les param�tres
	float3 N = normalize(vs.Norm);
	float3 L = normalize(vs.vDirLum);
	float3 V = normalize(vs.vDirCam);

	// Valeur de la composante diffuse
	float3 diff = saturate(dot(N, L));

	// R = 2 * (N.L) * N � L
	float3 R = normalize(2 * diff * N - L);
 
	// Calcul de la sp�cularit� 
	float3 S = pow(saturate(dot(R, V)), 4.0f);

	float3 couleurTexture;

	if (bTex>0)
	{
		// �chantillonner la couleur du pixel � partir de la texture
		couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;

		// I = A + D * N.L + (R.V)n
		couleur = couleurTexture * vAEcl.rgb +
				   couleurTexture * vDEcl.rgb * diff +
					vSEcl.rgb * vSMat.rgb * S;
	}
	else
	{
		couleur = vAEcl.rgb * vAMat.rgb + vDEcl.rgb * vDMat.rgb * diff +
			vSEcl.rgb * vSMat.rgb * S;
	}
	return float4(Lights[0].Color, 1.0f);
}

technique11 MiniPhong
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, MiniPhongVS()));
		SetPixelShader(CompileShader(ps_4_0, MiniPhongPS()));
		SetGeometryShader(NULL);
	}
}
