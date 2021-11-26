#pragma once

struct ILight {};

struct BaseLight : public ILight
{
	DirectX::XMFLOAT3 Color{ 0.f, 1.f, 0.f };
	float Intensity{ 0.0f };
};

using AmbiantLight = BaseLight;

struct DirectionalLight : public BaseLight {
	DirectX::XMFLOAT3 Direction;
	float _DataAlign_{ -1.f };
};

struct PointLight : public BaseLight {
	DirectX::XMFLOAT3 Position;
	float Range;
};

struct SpotLight : public BaseLight {
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Direction;
	float Angle;
	float Range;
};

// Store the largest light structure's size. Useful for bufferd size and stride config
constexpr std::size_t MAX_LIGHT_BYTE_WIDTH = sizeof(SpotLight);
