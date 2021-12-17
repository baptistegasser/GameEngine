struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 CoordTex : TEXCOORD0;
};

Texture2D Texture;
SamplerState SampleState;

VSOutput SharpenVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0)
{
    VSOutput Output = (VSOutput) 0;
    Output.Pos = Pos;
    Output.CoordTex = CoordTex;
    return Output;
}

float4 SharpenPS(VSOutput vs) : SV_Target
{
    float SharpenAmount = 25.0f;

    float4 Color = Texture.Sample(SampleState, vs.CoordTex);
    Color += Texture.Sample(SampleState, vs.CoordTex - 0.0003f) * SharpenAmount;
    Color -= Texture.Sample(SampleState, vs.CoordTex + 0.0003f) * SharpenAmount;

    return Color;
}

technique11 Sharpen
{
    pass p0
    {
        VertexShader = compile vs_5_0 SharpenVS();
        PixelShader = compile ps_5_0 SharpenPS();
        SetGeometryShader(NULL);
    }
}