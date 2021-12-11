#pragma once

#include "Light.h"
#include "core/Actor.h"

/// <summary>
/// Component for the directional light.
/// It cannot be used, you must use \ref{ ADirectionalLight }.
/// </summary>
class DirectionalLight : public LightComponent
{
    // Declare the constructor private so only the actor can be used
    friend class Pitbull::Actor;
    DirectionalLight(Pitbull::Actor *Parent);
};

/// <summary>
/// Actor to add a directional light to a scene.
/// </summary>
class ADirectionalLight : public Pitbull::Actor
{
public:
    ADirectionalLight();
    DirectionalLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    DirectionalLight* DirectionalLight;
};