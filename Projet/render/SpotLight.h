#pragma once

#include "Light.h"
#include "core/Actor.h"

class SpotLight : public LightComponent
{
public:
    SpotLight(Pitbull::Actor* Parent);
};

class ASpotLight : public Pitbull::Actor
{
public:
    ASpotLight();
    SpotLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    SpotLight* SpotLight;
};