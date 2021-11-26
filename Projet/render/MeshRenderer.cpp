#include "stdafx.h"
#include "MeshRenderer.h"

#include "MeshLoader.h"
#include "MoteurWindows.h"
#include "math/Math.h"
#include "ObjectMesh.h"

#include "util/ResourcesManager.h"

#include "Light.h"

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh)
	: MeshRenderer{ Parent, Mesh, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L"Default.fx")}
{}

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Shader* MeshShader)
	: Pitbull::Component{ Parent }
	, Mesh{ Mesh }
	, MeshShader{ MeshShader }
	, matWorld{ DirectX::XMMatrixIdentity() }
{
	TypeFlags |= RENDER_COMPONENT;
}

void MeshRenderer::Tick(const float& delta_time)
{
	// Update position
	matWorld = Math::TransformToMatrix(ParentActor->Transform);


	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(MeshShader->PInputLayout);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(Mesh->PIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Vertex buffer
	UINT stride = sizeof(PM3D::CSommetBloc);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &Mesh->PVertexBuffer, &stride, &offset);

	// Initialiser et sélectionner les «constantes» de l'effet
	const XMMATRIX& viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();

	ShaderParams.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	ShaderParams.matWorld = XMMatrixTranspose(matWorld);
	ShaderParams.vCamera = XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);


	// Update lighting

	std::vector<ILight*> SceneLights;
	SceneLights.push_back(new BaseLight{});

	BufferStruct BF{ 1.0f, 0.f, 0.f, 1.f };

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//  Disable GPU access to the vertex buffer data.
	pImmediateContext->Map(MeshShader->pStructuredBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//  Update the vertex buffer here.
	memcpy(mappedResource.pData, &BF, sizeof(BufferStruct));
	//  Reenable GPU access to the vertex buffer data.
	pImmediateContext->Unmap(MeshShader->pStructuredBuffer, 0);

	auto variableTexture = MeshShader->PEffect->GetVariableByName("Lights")->AsShaderResource();
	variableTexture->SetResource(MeshShader->pStructuredBufferView);

	// Dessiner les subsets non-transparents
	for (int32_t i = 0; i < Mesh->SubsetCount; ++i)
	{
		int32_t indexStart = Mesh->SubsetIndex[i];
		int32_t indexDrawAmount = Mesh->SubsetIndex[i + 1] - Mesh->SubsetIndex[i];
		if (indexDrawAmount)
		{
			ShaderParams.vAMat = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Ambiante);
			ShaderParams.vDMat = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Roughness);
			ShaderParams.vSMat = XMLoadFloat4(&Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Specular);
			ShaderParams.puissance = Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Power;

			// Activation de la texture ou non
			if (Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Texture->GetD3DTexture() != nullptr)
			{
				ID3DX11EffectShaderResourceVariable* variableTexture;
				variableTexture = MeshShader->PEffect->GetVariableByName("textureEntree")->AsShaderResource();
				variableTexture->SetResource(Mesh->Materials[Mesh->SubsetMaterialIndex[i]].Texture->GetD3DTexture());
				ShaderParams.bTex = 1;
			}
			else
			{
				ShaderParams.bTex = 1;
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
