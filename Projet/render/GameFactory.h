#pragma once

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

	void CreateTerrain();
	void CreatePlayer(Math::Vec3f Pos);
	void CreateEnemy();
	void CreatePlatform(Math::Vec3f Pos, Math::Vec3f Scale = { 1.f }, PhysicMaterial Material = Collider::DefaultMaterial);
	void CreateMobilePlatform(Math::Vec3f Pos, Math::Vec3f Scale, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material = Collider::DefaultMaterial);
	void CreateLights();

};
