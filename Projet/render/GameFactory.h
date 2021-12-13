#pragma once

#include "math/Transform.h"
#include "physic/Collider.h"
#include "util/Singleton.h"

class GameFactory : public Singleton<GameFactory>
{
	friend struct Singleton<GameFactory>;
public:
	void LoadLevel();

private:
	GameFactory() = default;
	~GameFactory() = default;

	/// <summary>
	/// Generate one terrain with Filename
	/// </summary>
	/// <param name="Filename"> : file name for map  </param>
	///	<param name="Pos"> : position of the map  </param>
	///	<param name="Scale"> : scale of the map  </param>
	void CreateTerrain(const wchar_t* Filename, Math::Transform Transform);

	/// <summary>
	/// Create the player
	/// </summary>
	/// <param name="Pos"> : init pos for the player  </param>
	void CreatePlayer(Math::Transform Transform);

	/// <summary>
	/// Create an enemy
	/// </summary>
	///	<param name="Pos"> : position of the enemy  </param>
	///	<param name="Scale"> : scale of the enemy  </param>
	void CreateEnemy(Math::Transform Transform);

	/// <summary>
	/// Create a fixed platform
	/// </summary>
	/// <param name="Pos"> : init position of platform </param>
	///	<param name="Filename"> : name of texture file </param>
	///	<param name="Scale"> : scale for platform </param>
	/// <param name="Material"> : material for platform </param>
	void CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material = Collider::DefaultMaterial);

	/// <summary>
	/// Create a moved platform
	/// </summary>
	/// <param name="Pos"> : init position of platform </param>
	///	<param name="End"> : vector direction of for platform movement </param>
	///	<param name="Filename"> : name of texture file </param>
	///	<param name="Scale"> : scale for platform </param>
	/// <param name="Material"> : material for platform </param>
	void CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material = Collider::DefaultMaterial);

	/// <summary>
	/// Create a light
	/// </summary>
	///	<param name="Pos"> : init position of the light </param>
	///	<param name="Specular"> : specular value of the light </param>
	///	<param name="Roughness"> : roughness value of the light </param>
	///	<param name="Intensity"> : intensity of the light </param>
	/// <param name="InnerRadius"> : innerRadius of the light </param>
	///	<param name="OuterRadius"> : outerRadius of the light </param>
	void CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity = 0.f, float InnerRadius = 0.f, float OuterRadius = 0.f);

};
