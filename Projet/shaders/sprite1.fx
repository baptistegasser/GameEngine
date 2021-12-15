cbuffer param
{
	float4x4 matWVP;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0;
};

VS_Sortie Sprite1VS(float4 Pos : POSITION, float2 coordTex : TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWVP);

	sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;
SamplerState SampleState;

float4 Sprite1PS(VS_Sortie vs) : SV_Target
{
float4 couleurTexture;

	couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);
	
	return couleurTexture;
}

technique11 AfficheSprite
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, Sprite1VS()));
		SetPixelShader(CompileShader(ps_4_0, Sprite1PS()));
		SetGeometryShader(NULL);
	}
}
