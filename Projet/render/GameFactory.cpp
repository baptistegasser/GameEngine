#include "stdafx.h"

#include "render/GameFactory.h"
#include "render/EngineD3D11.h"

// Physic components
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "physic/BoxCollider.h"
#include "physic/CapsuleCollider.h"

// Render components
#include "render/MeshRenderer.h"
#include "render/Camera.h"
#include "render/Terrain.h"

// Gameplay components
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"

void GameFactory::LoadLevel()
{
	CreateTerrain(L".\\modeles\\heigtmap\\Arene.bmp", Math::Transform{ Math::Vec3f{ 0.f, -50.f, 0.f }, Math::Vec3f{ 2.f, 1.f, 2.f } });
	CreateEnemy(Math::Vec3f{ 0.f, -7.f, 1.f });
	CreatePlayer(Math::Vec3f(0, 10.5f, 0));
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 0), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateMobilePlatform(Math::Vec3f(-15.f, 10.f, 25),  Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(15.f, 10.f, 35), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 60.f), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(0.f, 10.f, 105), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");

	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 130), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");


	//CreateMobilePlatform(Math::Vec3f(10.f, 10.f, 20.f), Math::Vec3f(1.f, 1.f, 1.f), Math::Vec3f(-10, 0, 0), L".\\modeles\\plateform\\plateformGlace.OMB");
	CreateLights(DirectX::XMFLOAT3{ 0.f, 20.f, 0.f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f },  4.f);
}

void GameFactory::CreateTerrain(const wchar_t* Filename, Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto Terrain = std::unique_ptr<ATerrain>(new ATerrain{
		Filename,
		{1, 0.3f, 1},
		Engine.ResourcesManager.GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
		PhysicMaterial{ 0.5f, 0.5f, 0.2f}
	});
	Terrain->Texture1 = Engine.ResourcesManager.GetTexture(L".\\modeles\\gazon.dds");
	Terrain->Texture2 = Engine.ResourcesManager.GetTexture(L".\\modeles\\roche.dds");
	Terrain->Texture3 = Engine.ResourcesManager.GetTexture(L".\\modeles\\chemin.dds");
	Terrain->Transform = std::move(Transform);
	Engine.GetScene().AddActor(std::move(Terrain), true);
}

void GameFactory::CreatePlayer(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();

	auto MyPlayer = Pitbull::Actor::New();
	MyPlayer->AddComponent<MeshRenderer>(Engine.ResourcesManager.GetMesh(L".\\modeles\\ball3\\ball.OMB"),
		Engine.ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlayer->AddComponent<Player>();
	MyPlayer->Transform = std::move(Transform);

	const auto PlayerCam = MyPlayer->AddComponent<Camera>(
		DirectX::XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.4f, 1.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
		&Engine.MatView,
		&Engine.MatProj,
		&Engine.MatViewProj);

	Engine.GetScene().SetCurrentCamera(PlayerCam);
	MyPlayer->Transform.RotateX(-90.0f);

	MyPlayer->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	const auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
	PlayerBody->SetMass(10.f);

	Engine.GetScene().AddActor(std::move(MyPlayer));
}

void GameFactory::CreateEnemy(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();

	auto Enemy = Pitbull::Actor::New();
	Enemy->AddComponent<MeshRenderer>(Engine.ResourcesManager.GetMesh(L".\\modeles\\cube\\cube.OMB"),
		Engine.ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	Enemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ 2.f, 0.f, 0.0f });
	Enemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ -2.f, 0.f, 0.0f });
	Enemy->Transform = std::move(Transform);

	auto body = Enemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Enemy->AddComponent<Plateform>(
		Math::Transform(Enemy->Transform.Position, Math::Quaternion(-physx::PxHalfPi, Math::Vec3f(0, 1, 0)))
		, Math::Transform(Enemy->Transform.Position + Math::Vec3f(10, 0, 0), Math::Quaternion(physx::PxHalfPi, Math::Vec3f(0, 1, 0))), true);
	Engine.GetScene().AddActor(std::move(Enemy));
}

void GameFactory::CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material)
{
	auto& Engine = EngineD3D11::GetInstance();

	auto MyPlatform = Pitbull::Actor::New();
	MyPlatform->AddComponent<MeshRenderer>(Engine.ResourcesManager.GetMesh(Filename),
		Engine.ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlatform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlatform->Transform = std::move(Transform);

	Engine.GetScene().AddActor(std::move(MyPlatform));
}

void GameFactory::CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material)
{
	auto& Engine = EngineD3D11::GetInstance();

	auto MyPlatform = Pitbull::Actor::New();
	MyPlatform->AddComponent<MeshRenderer>(Engine.ResourcesManager.GetMesh(Filename),
		Engine.ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlatform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlatform->Transform = Transform;

	MyPlatform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	MyPlatform->AddComponent<Plateform>(
		Math::Transform(MyPlatform->Transform.Position)
		, Math::Transform(MyPlatform->Transform.Position + End), true);
	Engine.GetScene().AddActor(std::move(MyPlatform));
}

void GameFactory::CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity, float InnerRadius, float OuterRadius)
{
	auto& Engine = EngineD3D11::GetInstance();

	Engine.GetScene().LightConfig.SetAmbient(AmbientLight{ 0.5f, 0.5f, 0.5f });
	PointLight Light{};
	Light.Intensity = Intensity;
	Light.InnerRadius = InnerRadius;
	Light.OuterRadius = OuterRadius;
	Light.Position = Pos;
	Light.Specular = Specular;
	Light.Roughness = Roughness;
	Engine.GetScene().LightConfig.AddPointLight(Light);
}