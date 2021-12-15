#pragma once

#include "core/Component.h"
#include "math/Vec3f.h"
#include "util/Util.h"

/// <summary>
/// Representation of a light, identical to the shader usage
/// </summary>
struct Light
{
    /// <summary>
    /// The type of light, impact the computation and resulting lighting
    /// </summary>
    enum class LightType
    {
        Spot,
        Point,
        Directional,
    } Type;

    /// <summary>
    /// The light color, default to white.
    /// </summary>
    Math::Vec3f Color{ 1.0f, 1.0f, 1.0f };
    /// <summary>
    /// The light position, used only for Spot and Point.
    /// </summary>
    Math::Vec3f Position{ 0.f, 0.f, 0.f };
    /// <summary>
    /// The direction the light is oriented, used only for Spot and Directional.
    /// </summary>
    Math::Vec3f Direction{ 0.f, -1.f , 0.f };
    /// <summary>
    /// The light maximum emission distance, used only for Spot and Point.
    /// </summary>
    float Range{ 1.f };
    /// <summary>
    /// The angle of the light, Spot only.
    /// </summary>
    float SpotAngle{ 30.f };
    /// <summary>
    /// The light intensity affecting the amount of lighting given.
    /// </summary>
    float Intensity{ 1.f };

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
    LightComponent(Pitbull::Actor* Parent, LightType Type)
		: Light{Type}, Component{Parent} {}
    ~LightComponent() override = default;

    /// <summary>
    /// Allow light component to register themself.
    /// </summary>
    void Init() override;
    /// <summary>
    /// Get the base data struct representing a light.
    /// </summary>
    const Light GetRawLight() const noexcept;
};
