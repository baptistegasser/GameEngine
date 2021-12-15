#include "stdafx.h"
#include "Light.h"

#include "MoteurWindows.h"

void LightComponent::Init()
{
	Component::Init();
	auto& Scene = PM3D::CMoteurWindows::GetInstance().GetScene();
	Scene.LightManager.RegisterLight(this);
}

const Light LightComponent::GetRawLight() const noexcept
{
	const auto ptr = static_cast<const Light*>(static_cast<const LightComponent*>(this));
    return *ptr;
}
