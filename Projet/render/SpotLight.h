#pragma once

#include "Light.h"
#include "core/Actor.h"

/// <summary>
/// Component for the spot light.
/// </summary>
class SpotLight : public LightComponent
{
public:
    SpotLight(Pitbull::Actor* Parent);
};

/// <summary>
/// Actor to easily add a single spot light to a scene.
/// </summary>
class ASpotLight : public Pitbull::Actor
{
public:
    ASpotLight();
    SpotLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    SpotLight* SpotLight;
};