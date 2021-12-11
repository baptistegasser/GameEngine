#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Pitbull::Actor* Parent)
	: LightComponent{ Parent, LightType::Directionnal }
{}

ADirectionalLight::ADirectionalLight()
	: Actor{}
{
	DirectionalLight = AddComponent<::DirectionalLight>();
}

DirectionalLight* ADirectionalLight::GetLight() noexcept
{
	return DirectionalLight;
}
