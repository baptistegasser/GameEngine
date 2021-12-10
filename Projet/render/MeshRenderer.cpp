#include "stdafx.h"
#include "MeshRenderer.h"

#include "MeshLoader.h"
#include "EngineD3D11.h"
#include "math/Math.h"
#include "ObjectMesh.h"
#include "Vertex.h"

#include "util/ResourcesManager.h"

#include "Light.h"

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh)
	: MeshRenderer{ Parent, Mesh, EngineD3D11::GetInstance().ResourcesManager.GetShader(L"Default.fx")}
{}

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Shader* MeshShader)
	: Component{ Parent }
	, matWorld{DirectX::XMMatrixIdentity()}
	, Mesh{Mesh}
	, MeshShader{MeshShader}
{}

void MeshRenderer::LateTick(const float& ElapsedTime)
{
	using namespace DirectX;

	// Update position
	matWorld = ParentActor->Transform;

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = EngineD3D11::GetInstance().Device->ImmediateContext;
	const auto& LightConfig = EngineD3D11::GetInstance().GetScene().LightConfig;

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
	const XMMATRIX& viewProj = EngineD3D11::GetInstance().MatViewProj;

	ShaderParams.MatWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	ShaderParams.MatWorld = XMMatrixTranspose(matWorld);
	ShaderParams.CameraPos = EngineD3D11::GetInstance().GetScene().GetCurrentCamera().GetPosition();

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);

	// Update lighting
	if (LightConfig.Changed()) {
		MeshShader->UpdateLightsBuffer(pImmediateContext, LightConfig);
	}
	ShaderParams.Ambient = LightConfig.GetAmbient();
	ShaderParams.Directional = LightConfig.GetDirectional();

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
}
