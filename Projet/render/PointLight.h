#pragma once

#include "Light.h"
#include "core/Actor.h"

class PointLight : public LightComponent
{
public:
    PointLight(Pitbull::Actor *Parent);
};

class APointLight : public Pitbull::Actor
{
public:
    APointLight();
    PointLight* GetLight() noexcept;

private:
    using Actor::AddComponent;
    PointLight* PointLight;
};
