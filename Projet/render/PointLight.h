#pragma once

#include "Light.h"
#include "core/Actor.h"

/// <summary>
/// Component for the point light.
/// </summary>
class PointLight : public LightComponent
{
public:
    PointLight(Pitbull::Actor *Parent);
};

/// <summary>
/// Actor to easily add a single point light to a scene.
/// </summary>
class APointLight : public Pitbull::Actor
{
public:
    APointLight();
    PointLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    PointLight* PointLight;
};
