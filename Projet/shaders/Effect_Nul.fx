struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 CoordTex : TEXCOORD0;
};

Texture2D Texture;
SamplerState SampleState;

VSOutput NulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0)
{
    VSOutput Output = (VSOutput) 0;
    Output.Pos = Pos;
    Output.CoordTex = CoordTex;
    return Output;
}

float4 NulPS(VSOutput vs) : SV_Target
{
    float4 Color;
    Color = Texture.Sample(SampleState, vs.CoordTex);
    return Color + float4(0.2f, 0.f, 0.f, 0.f);
}

technique11 Nul
{
    pass p0
    {
        VertexShader = compile vs_5_0 NulVS();
        PixelShader = compile ps_5_0 NulPS();
        SetGeometryShader(NULL);
    }
}