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
    float distance = 0.1f;

    float4 couleur;
    float4 ct;
    float2 tc = vs.CoordTex;
    float d, dx, dy;
    couleur = 0;
    float x = tc.x * 2 - 1.f;
    float y = tc.y * 2 - 1.f;
    dx = sqrt(x * x);
    dy = sqrt(y * y);
    dx = x * (distance * dx);
    dy = y * (distance * dy);
    x = x - (dx * 10);
    y = y - (dy * 10);
    tc.x = (x + 1.0) / 2.0;
    tc.y = (y + 1.0) / 2.0;
    for (int i = 0; i < 10; i++)
    {
        ct = Texture.Sample(SampleState, tc);
        couleur = couleur * 0.6 + ct * 0.4;
        x = x + dx;
        y = y + dy;
        tc.x = (x + 1.0) / 2.0;
        tc.y = (y + 1.0) / 2.0;
    }
    return couleur;
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