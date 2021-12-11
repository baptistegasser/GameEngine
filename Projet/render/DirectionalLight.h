#pragma once

#include "Light.h"
#include "core/Actor.h"

class DirectionalLight : public LightComponent
{
    // Declare the constructor private so only the actor can be used
    friend class Pitbull::Actor;
    DirectionalLight(Pitbull::Actor *Parent);
};

class ADirectionalLight : public Pitbull::Actor
{
public:
    ADirectionalLight();
    DirectionalLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    DirectionalLight* DirectionalLight;
};