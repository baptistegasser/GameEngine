#include "stdafx.h"

#include "render/GameFactory.h"
#include "render/MoteurWindows.h"

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
	auto Terrain = std::unique_ptr<ATerrain>(new ATerrain{
			Filename,
			{1, 0.3f, 1},
			PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
		});
	Terrain->Texture1 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\gazon.dds");
	Terrain->Texture2 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\roche.dds");
	Terrain->Texture3 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\chemin.dds");
	Terrain->Transform = Transform;
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Terrain), true);
}

void GameFactory::CreatePlayer(Math::Transform Transform)
{
	auto MyPlayer = Pitbull::Actor::New();
	MyPlayer->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\ball3\\ball.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlayer->AddComponent<Player>();
	MyPlayer->Transform = Transform;

	auto PlayerCam = MyPlayer->AddComponent<Camera>(DirectX::XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 0.4f, 1.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatView(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatProj(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatViewProj());

	PM3D::CMoteurWindows::GetInstance().GetScene().SetCurrentCamera(PlayerCam);
	MyPlayer->Transform.RotateX(-90.0f);

	MyPlayer->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
	PlayerBody->SetMass(10.f);

	PM3D::CMoteurWindows::GetInstance().SetSkyBox(new Skybox{
	&MyPlayer->Transform, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\sky\\sky.OMB"),
	PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx") });

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlayer));


}

void GameFactory::CreateEnemy(Math::Transform Transform)
{
	auto Ennemy = Pitbull::Actor::New();
	Ennemy->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\cube\\cube.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ 2.f, 0.f, 0.0f });
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ -2.f, 0.f, 0.0f });
	Ennemy->Transform = Transform;

	auto body = Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<Plateform>(
		Math::Transform(Ennemy->Transform.Position, Math::Quaternion(-physx::PxHalfPi, Math::Vec3f(0, 1, 0)))
		, Math::Transform(Ennemy->Transform.Position + Math::Vec3f(10, 0, 0), Math::Quaternion(physx::PxHalfPi, Math::Vec3f(0, 1, 0))), true);
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Ennemy));
}

void GameFactory::CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(Filename),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(Filename),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;

	MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	MyPlateform->AddComponent<Plateform>(
		Math::Transform(MyPlateform->Transform.Position)
		, Math::Transform(MyPlateform->Transform.Position + End), true);
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity, float InnerRadius, float OuterRadius)
{
	PM3D::CMoteurWindows::GetInstance().GetScene().LightConfig.SetAmbient(AmbientLight{ 0.5f, 0.5f, 0.5f });
	PointLight Light;
	Light.Intensity = Intensity;
	Light.InnerRadius = InnerRadius;
	Light.OuterRadius = OuterRadius;
	Light.Position = Pos;
	Light.Specular = Specular;
	Light.Roughness = Roughness;
	PM3D::CMoteurWindows::GetInstance().GetScene().LightConfig.AddPointLight(Light);
}