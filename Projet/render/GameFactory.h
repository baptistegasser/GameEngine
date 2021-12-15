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
	///	<param name="Transform"> : transform of the map  </param>
	void CreateTerrain(const wchar_t* Filename, Math::Transform Transform, const std::wstring& TextureName1, const std::wstring& TextureName2, const std::wstring& TextureName3);

	/// <summary>
	/// Create the player
	/// </summary>
	/// <param name="Transform"> : init transform for the player  </param>
	void CreatePlayer(Math::Transform Transform);

	/// <summary>
	/// Create an enemy
	/// </summary>
	///	<param name="Transform"> : transform of the enemy  </param>
	///	<param name="End"> : deplacement of the enemy  </param>
	void CreateEnemy(Math::Transform Transform, Math::Vec3f End, bool IsKiller = false, float Speed = 0.1f);

	/// <summary>
	/// Create an intelligent enemy
	/// Follow the actor's tranform if the distance is under Distance
	/// </summary>
	///	<param name="Transform"> : transform of the enemy  </param>
	/// <param name="ToFollow"> transform of the actor we want to follow </param>
	/// /// <param name="Distance"> distance max the enemy follow the actor's tranform </param>
	void CreateIntelligentEnemy(Math::Transform Transform, Math::Transform* ToFollow, float Distance = std::numeric_limits<float>::infinity(), bool IsKiller = false, float Speed = 0.1f);

	/// <summary>
	/// Create a fixed platform
	/// </summary>
	/// <param name="Transform"> : init transform of platform </param>
	///	<param name="Filename"> : name of texture file </param>
	/// <param name="Material"> : material for platform </param>
	void CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material = Collider::DefaultMaterial);

	/// <summary>
	/// Create a moved platform
	/// </summary>
	/// <param name="Transform"> : init transform of platform </param>
	///	<param name="End"> : vector direction of for platform movement </param>
	///	<param name="Filename"> : name of texture file </param>
	/// <param name="Material"> : material for platform </param>
	void CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material = Collider::DefaultMaterial, float Speed = 0.1f);

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

	/// <summary>
	/// Create a skybox
	/// </summary>
	/// <param name="ToFollow"> The actor's transform the skybox will follow </param>
	void CreateSkyBox(Math::Transform* ToFollow);

	void CreateTunnel(Math::Transform Transform);
private :
	Math::Transform* PlayerTransform;
	/// <summary>
	/// Create an CheckPoint to respawn a player
	/// </summary>
	///	<param name="Transform"> : the transform of the checkPoint  </param>
	void CreateCheckPoint(Math::Transform Transform);
};
