#include "stdafx.h"
#include "Light.h"

#include "EngineD3D11.h"

void LightComponent::Init()
{
	Component::Init();
	auto& Scene = EngineD3D11::GetInstance().GetScene();
	Scene.LightManager.RegisterLight(this);
}

const Light LightComponent::GetRawLight() const noexcept
{
	const auto ptr = static_cast<const Light*>(static_cast<const LightComponent*>(this));
    return *ptr;
}
