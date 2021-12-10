#pragma once

#include "core/Component.h"
#include "math/Vec3f.h"
#include "util/Util.h"

/// <summary>
/// Representation of a light, identical to the shader usage
/// </summary>
struct Light
{
    enum class LightType
    {
        Spot,
        Point,
        Directionnal,
    } Type;

    Math::Vec3f Color;
    Math::Vec3f Position;
    Math::Vec3f Direction;
    float Range;
    float SpotAngle;
    float Intensity;

    DX_HLSL_FILL(3);
};

DX_HLSL_ASSERT_ALLIGN(Light);

/// <summary>
/// Special class to create a light implementation that is a Component
/// but can still be cast to the raw light struct for HLSL.
/// </summary>
class LightComponent : public Light, public Pitbull::Component
{
public:
    LightComponent(Pitbull::Actor* Parent, LightType Type) : Component{Parent}, Light{ Type } {}

    void Init() override;
    const Light* GetRawLightPtr() const noexcept;
    const Light GetRawLight() const noexcept;
};
