#include "stdafx.h"
#include "MeshRenderer.h"

#include "MeshLoader.h"
#include "MoteurWindows.h"
#include "math/Math.h"
#include "ObjectMesh.h"

#include "util/ResourcesManager.h"

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Light& Light)
	: MeshRenderer{ Parent, Mesh, Light, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L"Default.fx")}
{}

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Light& Light, Shader* MeshShader)
	: Pitbull::Component{ Parent }
	, Mesh{ Mesh }
	, MeshShader{ MeshShader }
	, matWorld{ DirectX::XMMatrixIdentity() }
	, LightShader1{ Light }
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
	ShaderParams.vLumiere = LightShader1.Position;
	ShaderParams.vCamera = XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);
	ShaderParams.vAEcl = LightShader1.Ambiante;
	ShaderParams.vDEcl = LightShader1.Roughness;
	ShaderParams.vSEcl = LightShader1.Specular;

	LightParams.Position = LightShader1.Position;
	LightParams.Ambiante = LightShader1.Ambiante;
	LightParams.Roughness = LightShader1.Roughness;
	LightParams.Specular = LightShader1.Specular;

	//LightParams3.Position = LightShader1.Position;
	//LightParams3.Ambiante = LightShader1.Ambiante;
	//LightParams3.Roughness = LightShader1.Roughness;
	//LightParams3.Specular = LightShader1.Specular;


	LightShader blop[1] = { LightShader1.Position, LightShader1.Ambiante, LightShader1.Roughness, LightShader1.Specular };

	/*LightShader* blap = new LightShader[1];
	blap[1] = { LightShader1.Position, LightShader1.Ambiante, LightShader1.Roughness, LightShader1.Specular };*/


	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);

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

			// 2e buffer
			ID3DX11EffectConstantBuffer* pCB2 = MeshShader->PEffect->GetConstantBufferByName("lights");
			pCB2->SetConstantBuffer(MeshShader->PConstantBuffer2);
			pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer2, 0, nullptr, &LightParams, 0, 0);

			//pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);

			// 3e

			D3D11_MAPPED_SUBRESOURCE mappedResource2;
			auto result = pImmediateContext->Map((ID3D11Resource* ) MeshShader->PStructuredBuffer3, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);

			size_t sizeInBytes = sizeof(blop);

			memcpy_s(mappedResource2.pData, sizeInBytes, blop, sizeInBytes);

			pImmediateContext->Unmap(MeshShader->PStructuredBuffer3, 0);

			ID3DX11EffectShaderResourceVariable* ress = MeshShader->PEffect->GetVariableByName("lightsSt")->AsShaderResource();
			ress->SetResource(MeshShader->ShaderResourceView);

			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);

			
		}
	}
}
