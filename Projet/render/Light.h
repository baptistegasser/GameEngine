#pragma once

struct BaseLight
{
	DirectX::XMFLOAT3 Color{ 255.f, 255.f, 255.f };
	float Intensity{ 1.0f };
};

struct AmbiantLight : public BaseLight {};

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
