#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SpotLight(Pitbull::Actor* Parent)
	: LightComponent{ Parent, LightType::Spot }
{}

ASpotLight::ASpotLight()
	: Actor{}
{
	SpotLight = AddComponent<::SpotLight>();
}

SpotLight* ASpotLight::GetLight() noexcept
{
	return SpotLight;
}
