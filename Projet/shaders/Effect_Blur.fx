struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 CoordTex : TEXCOORD0;
};

Texture2D Texture;
SamplerState SampleState;

VSOutput BlurVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0)
{
    VSOutput Output = (VSOutput) 0;
    Output.Pos = Pos;
    Output.CoordTex = CoordTex;
    return Output;
}

float4 BlurPS(VSOutput vs) : SV_Target
{
    float2 Offset = 0.01f;
    float4 Color;
    Color = Texture.Sample(SampleState, float2(vs.CoordTex.x + Offset.x , vs.CoordTex.y + Offset.y));
    Color += Texture.Sample(SampleState, float2(vs.CoordTex.x - Offset.x, vs.CoordTex.y - Offset.y));
    Color += Texture.Sample(SampleState, float2(vs.CoordTex.x + Offset.x, vs.CoordTex.y - Offset.y));
    Color += Texture.Sample(SampleState, float2(vs.CoordTex.x - Offset.x, vs.CoordTex.y + Offset.y));

    return Color/4;
}

technique11 Blur
{
    pass p0
    {
        VertexShader = compile vs_5_0 BlurVS();
        PixelShader = compile ps_5_0 BlurPS();
        SetGeometryShader(NULL);
    }
}