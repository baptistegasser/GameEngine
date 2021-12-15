#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight(Pitbull::Actor* Parent)
	: LightComponent{ Parent, LightType::Point }
{}

APointLight::APointLight()
	: Actor{}
{
	PointLight = AddComponent<::PointLight>();
}

PointLight* APointLight::GetLight() noexcept
{
	return PointLight;
}
