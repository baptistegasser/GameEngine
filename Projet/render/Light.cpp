#include "stdafx.h"
#include "Light.h"

#include "MoteurWindows.h"

void LightComponent::Init()
{
	auto& Scene = PM3D::CMoteurWindows::GetInstance().GetScene();
	Scene.LightManager.RegisterLight(this);
}

const Light* LightComponent::GetRawLightPtr() const noexcept
{
    return static_cast<const Light*>(static_cast<const LightComponent*>(this));
}

const Light LightComponent::GetRawLight() const noexcept
{
    return *GetRawLightPtr();
}
