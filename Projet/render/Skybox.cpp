#include "stdafx.h"
#include "Skybox.h"
#include "MeshLoader.h"
#include "MoteurWindows.h"
#include "math/Math.h"
#include "ObjectMesh.h"
#include "Vertex.h"
#include "Light.h"

Skybox::Skybox(Math::Transform* ToFollow, ObjectMesh* Mesh)
	: Skybox{ ToFollow, Mesh, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L"Default.fx")}
{}

Skybox::Skybox(Math::Transform* ToFollow, ObjectMesh* Mesh, Shader* MeshShader)
	: ToFollow{ ToFollow }
	, Mesh { Mesh }
	, MeshShader{ MeshShader }
	, matWorld{ DirectX::XMMatrixIdentity() }
{}

void Skybox::LateTick(const float ElapsedTime)
{
	using namespace DirectX;

	PM3D::CMoteurWindows::GetInstance().GetDispositif().DeactivateZBuffer();
	// Update position
	matWorld = XMMatrixTranslationFromVector(ToFollow->Position.ToXMVector());

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = PM3D::CMoteurWindows::GetInstance().GetDispositif().ImmediateContext;


	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(MeshShader->PInputLayout);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(Mesh->PIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &Mesh->PVertexBuffer, &stride, &offset);

	// Initialiser et s�lectionner les �constantes� de l'effet
	const XMMATRIX& viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();

	ShaderParams.MatWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	ShaderParams.MatWorld = XMMatrixTranspose(matWorld);
	ShaderParams.CameraPos = PM3D::CMoteurWindows::GetInstance().GetScene().GetCurrentCamera().GetPosition();

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);

	// Set lighting data
	const auto& LightManager = PM3D::CMoteurWindows::GetInstance().GetScene().LightManager;
	ShaderParams.AmbientColor = LightManager.AmbientColor.ToXMVector();
	MeshShader->UpdateLightsBuffer(pImmediateContext);

	// Dessiner les subsets non-transparents
	for (int32_t i = 0; i < Mesh->SubsetCount; ++i)
	{
		int32_t indexStart = Mesh->SubsetIndex[i];
		int32_t indexDrawAmount = Mesh->SubsetIndex[i + 1] - Mesh->SubsetIndex[i];
		if (indexDrawAmount)
		{
			ShaderParams.Mat.Ambient = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Ambiante);
			ShaderParams.Mat.Roughness = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Roughness);
			ShaderParams.Mat.Specular = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Specular);
			ShaderParams.Mat.Intensity = Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Power;

			// Activation de la texture ou non
			if (Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Texture->TextureView != nullptr)
			{
				ID3DX11EffectShaderResourceVariable* variableTexture;
				variableTexture = MeshShader->PEffect->GetVariableByName("textureEntree")->AsShaderResource();
				ShaderParams.HasTexture = true;
				variableTexture->SetResource(Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Texture->TextureView);
			}
			else
			{
				ShaderParams.HasTexture = false;
			}

			// IMPORTANT pour ajuster les param.
			MeshShader->PEffectPass->Apply(0, pImmediateContext);

			ID3DX11EffectConstantBuffer* pCB = MeshShader->PEffect->GetConstantBufferByName("param");
			pCB->SetConstantBuffer(MeshShader->PConstantBuffer);
			pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);

			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}

	PM3D::CMoteurWindows::GetInstance().GetDispositif().ActivateZBuffer();
}
