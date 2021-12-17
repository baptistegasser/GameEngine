struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 CoordTex : TEXCOORD0;
};

Texture2D Texture;
SamplerState SampleState;

VSOutput RadialBlurVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD0)
{
    VSOutput Output = (VSOutput)0;
    Output.Pos = Pos;
    Output.CoordTex = CoordTex;
    return Output;
}

float4 RadialBlurPS(VSOutput vs) : SV_Target
{
    float Distance = 0.1f;

    float4 Color;
    float4 CoordT;
    float2 TCoord = vs.CoordTex;
    float Dx, Dy;
    Color = 0;
    float X = TCoord.x * 2 - 1.f;
    float Y = TCoord.y * 2 - 1.f;
    Dx = sqrt(X * X);
    Dy = sqrt(Y * Y);
    Dx = X * (Distance * Dx);
    Dy = Y * (Distance * Dy);
    X = X - (Dx * 10);
    Y = Y - (Dy * 10);
    TCoord.x = (X + 1.0) / 2.0;
    TCoord.y = (Y + 1.0) / 2.0;
    for (int i = 0; i < 10; i++)
    {
        CoordT = Texture.Sample(SampleState, TCoord);
        Color = Color * 0.6 + CoordT * 0.4;
        X = X + Dx;
        Y = Y + Dy;
        TCoord.x = (X + 1.0) / 2.0;
        TCoord.y = (Y + 1.0) / 2.0;
    }
    return Color;
}

technique11 RadialBlur
{
    pass p0
    {
        VertexShader = compile vs_5_0 RadialBlurVS();
        PixelShader = compile ps_5_0 RadialBlurPS();
        SetGeometryShader(NULL);
    }
}