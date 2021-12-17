struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 CoordTex : TEXCOORD0;
};

Texture2D Texture;
SamplerState SampleState;

VSOutput WavesVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0)
{
    VSOutput Output = (VSOutput) 0;
    Output.Pos = Pos;
    Output.CoordTex = CoordTex;
    return Output;
}

float4 WavesPS(VSOutput vs) : SV_Target
{
    float Scale = 20.f;
    float Magnitude = 0.01f;
    float Offset = 0.1f;

    float2 TexCoord;
    TexCoord = vs.CoordTex + sin(Scale * (vs.CoordTex.x + Offset)) * Magnitude;

    float4 Color = Texture.Sample(SampleState, TexCoord);

    return Color;
}

technique11 Waves
{
    pass p0
    {
        VertexShader = compile vs_5_0 WavesVS();
        PixelShader = compile ps_5_0 WavesPS();
        SetGeometryShader(NULL);
    }
}