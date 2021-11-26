#include "stdafx.h"
#include "LightConfig.h"

void LightConfig::SetAmbient(const AmbientLight& NewAmbient)
{
	this->Ambient = NewAmbient;
	ConfigChanged = true;
}

void LightConfig::SetDirectional(const DirectionalLight& NewDirectional)
{
	this->Directional = NewDirectional;
	ConfigChanged = true;
}

void LightConfig::AddPointLight(const PointLight& Light)
{
	this->PointLights.push_back(Light);
	ConfigChanged = true;
}

bool LightConfig::Changed() const noexcept
{
	return ConfigChanged;
}

const AmbientLight& LightConfig::GetAmbient() const noexcept
{
	return Ambient;
}

const DirectionalLight& LightConfig::GetDirectional() const noexcept
{
	return Directional;
}

const std::vector<PointLight>& LightConfig::GetPointLights() const noexcept
{
	return PointLights;
}
