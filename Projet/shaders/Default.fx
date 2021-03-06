cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vLumiere; 		// la position de la source d'?clairage (Point)
	float4 vCamera; 			// la position de la cam?ra
	float4 vAEcl; 			// la valeur ambiante de l'?clairage
	float4 vAMat; 			// la valeur ambiante du mat?riau
	float4 vDEcl; 			// la valeur diffuse de l'?clairage 
	float4 vDMat; 			// la valeur diffuse du mat?riau 
	float4 vSEcl; 			// la valeur sp?culaire de l'?clairage 
	float4 vSMat; 			// la valeur sp?culaire du mat?riau 
	float puissance;
	int bTex;		    // Bool?en pour la pr?sence de texture
	float2 remplissage;

	//vector<float4, 4> Lights;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 vDirLum : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
};

VS_Sortie DefaultVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex: TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), matWorld).xyz;

	float3 PosWorld = mul(Pos, matWorld).xyz;

	sortie.vDirLum = vLumiere.xyz - PosWorld;
	sortie.vDirCam = vCamera.xyz - PosWorld;

	// Coordonn?es d'application de texture
	sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'?tat de sampling

float4 DefaultPS( VS_Sortie vs ) : SV_Target
{
	float3 couleur;

	// Normaliser les param?tres
	float3 N = normalize(vs.Norm);
	float3 L = normalize(vs.vDirLum);
	float3 V = normalize(vs.vDirCam);

	// Valeur de la composante diffuse
	float3 diff = saturate(dot(N, L));

	// R = 2 * (N.L) * N ? L
	float3 R = normalize(2 * diff * N - L);
 
	// Calcul de la sp?cularit? 
	float3 S = pow(saturate(dot(R, V)), 4.0f);

	float3 couleurTexture;

	if (bTex>0)
	{
		// ?chantillonner la couleur du pixel ? partir de la texture
		couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;

		// I = A + D * N.L + (R.V)n
		couleur =  couleurTexture * vAEcl.rgb  + 
				   couleurTexture * vDEcl.rgb * diff +
				   vSEcl.rgb * vSMat.rgb * S;
	}
	else
	{
		couleur =  vAEcl.rgb * vAMat.rgb + vDEcl.rgb * vDMat.rgb * diff  +
					vSEcl.rgb * vSMat.rgb * S;
	}
	return float4(couleur, 1.0f);
}

technique11 Default
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, DefaultVS()));
		SetPixelShader(CompileShader(ps_4_0, DefaultPS()));
		SetGeometryShader(NULL);
	}
}
