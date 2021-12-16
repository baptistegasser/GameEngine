#pragma once

#include "core/Component.h"
#include "render/Terrain.h"
#include <limits>

#include "Enemy.h"

class IntelligentEnemy : public Enemy
{
public :
	struct ActionZone {
		Math::Vec3f Point1, Point2;
	};

	IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow, ActionZone Zone, Math::Vec3f BasePosition, ATerrain* RelativeTerrain, 
		Math::Vec3f RelativeTerrainPosition, float Distance = std::numeric_limits<float>::infinity(), bool IsKiller = false);
	~IntelligentEnemy() override = default;

	void Init() override;

	/// <summary>
	/// Update the position and rotation of the enemy
	/// Follow the actor's transform only if he is at a distance under Distance
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	void SetSpeed(const float& SpeedNew);

	
private:
	Math::Transform* ToFollow;
	Math::Vec3f Direction;
	Math::Vec3f BasePosition;
	const ATerrain* RelativeTerrain;
	Math::Vec3f RelativeTerrainPosition;

	float Speed = 0.1f;
	float Distance;

	ActionZone Zone;
};

