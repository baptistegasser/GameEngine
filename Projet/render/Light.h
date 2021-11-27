#pragma once

struct AmbientLight {
	DirectX::XMVECTOR Value;

    AmbientLight() : AmbientLight(1.0f, 1.0f, 1.0f) {}
    AmbientLight(float r, float g, float b) : Value{r,g,b,1.0f} {}
};

struct DirectionalLight
{
    DirectX::XMVECTOR Direction;
    DirectX::XMVECTOR Specular;
    DirectX::XMVECTOR Roughness;
};

struct PointLight
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Specular;
    DirectX::XMFLOAT3 Roughness;
    float InnerRadius, OuterRadius, Intensity;
};