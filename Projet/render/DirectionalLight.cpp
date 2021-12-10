#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Pitbull::Actor* Parent)
	: LightComponent{ Parent, LightType::Directionnal }
{}

ADirectionalLight::ADirectionalLight()
	: Actor{}
{
	AddComponent<DirectionalLight>();
}
