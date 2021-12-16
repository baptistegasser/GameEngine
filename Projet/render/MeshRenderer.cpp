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

	auto& Engine = EngineD3D11::GetInstance();

	// Update position
	matWorld = ParentActor->Transform;

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = Engine.Device->ImmediateContext;

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(Mesh->PIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &Mesh->PVertexBuffer, &stride, &offset);

	MeshShader->PEffect->GetVariableByName("ShadowTexture")->Release();
	MeshShader->PEffectPass->Apply(0, pImmediateContext);

	RenderShadowMap();

	// Cleanup after shadow depth render
	ID3D11RenderTargetView* tabRTV[1];
	tabRTV[0] = Engine.Device->RenderTargetView;
	pImmediateContext->OMSetRenderTargets(1, tabRTV, Engine.Device->DepthStencilView);

	Engine.Device->ResetViewPortDimension();
	MeshShader->PEffectTechnique = MeshShader->PEffect->GetTechniqueByName("MiniPhong");
	MeshShader->PEffectPass = MeshShader->PEffectTechnique->GetPassByIndex(0);

	ShadersParams ShaderParams;
	ShaderParams.MatWorldViewProj = XMMatrixTranspose(matWorld * Engine.MatViewProj);
	ShaderParams.MatWorld = XMMatrixTranspose(matWorld);
	ShaderParams.CameraPos = Engine.GetScene().GetCurrentCamera().GetPosition();

	// Set ambient
	const auto& LightManager = EngineD3D11::GetInstance().GetScene().LightManager;
	ShaderParams.AmbientColor = LightManager.AmbientColor.ToXMVector();
	MeshShader->UpdateLightsBuffer();

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);

	pImmediateContext->IASetInputLayout(MeshShader->PInputLayout);

	ID3DX11EffectShaderResourceVariable* PShadowMap;
	PShadowMap = MeshShader->PEffect->GetVariableByName("ShadowTexture")->AsShaderResource();
	PShadowMap->SetResource(MeshShader->PDepthShaderResourceView);

	Engine.Device->SetNormalRSState();

	ID3DX11EffectConstantBuffer* pCB = MeshShader->PEffect->GetConstantBufferByName("param");

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

			pCB->SetConstantBuffer(MeshShader->PConstantBuffer);
			pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);

			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}
}

void MeshRenderer::RenderShadowMap()
{
	auto& PDevice = EngineD3D11::GetInstance().Device;
	ID3D11DeviceContext* pImmediateContext = PDevice->ImmediateContext;
	// Set texture as render surface and clear it
	//ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	//pImmediateContext->PSSetShaderResources(2, 1, pSRV);
	PDevice->SetRenderTargetView(nullptr, MeshShader->PDepthStencilView);
	//pImmediateContext->OMSetRenderTargets(0, nullptr, MeshShader->PDepthStencilView);
	//pImmediateContext->ClearDepthStencilView(MeshShader->PDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Depth texture as different viewport size
	PDevice->SetViewPortDimension(512, 512);

	// Load shadow technique
	MeshShader->PEffectTechnique = MeshShader->PEffect->GetTechniqueByName("ShadowMap");
	MeshShader->PEffectPass = MeshShader->PEffectTechnique->GetPassByIndex(0);

	pImmediateContext->IASetInputLayout(MeshShader->PVertexLayoutShadow);

	using namespace DirectX;
	const auto& DirectionalLight = EngineD3D11::GetInstance().GetScene().LightManager.GetDirectionalLight();
	// Matrice de la vision vu par la lumière - Le point TO est encore 0,0,0
	auto ViewMat = XMMatrixLookAtLH(XMVectorSet(0.f, 11.0f, 0.0f, 1.0f),
					XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
					XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f));
	
	const static float FOV = XM_PI / 4;
	const static float AspectRatio = 1.f;
	const static float NearPlan = 1.0f;
	const static float FarPlan = 1000.f;
	auto ProjMat = XMMatrixPerspectiveFovLH(FOV, AspectRatio, NearPlan, FarPlan);
	
	auto MVPLight = ViewMat * ProjMat;

	// Only care about light view matrix
	ShadersParams sp;
	sp.LightMatWorldViewProj = DirectX::XMMatrixTranspose(matWorld * EngineD3D11::GetInstance().MatViewProj);
	// Only one buffer cared
	ID3DX11EffectConstantBuffer* pCB = MeshShader->PEffect->GetConstantBufferByName("param");
	pCB->SetConstantBuffer(MeshShader->PConstantBuffer);
	pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer, 0, nullptr, &sp, 0, 0);

	// Draw subets
	for (int i = 0; i < Mesh->SubsetCount; ++i)
	{
		const int indexStart = Mesh->SubsetIndex[i];
		const int indexDrawAmount = Mesh->SubsetIndex[i + 1] - Mesh->SubsetIndex[i];
		if (indexDrawAmount)
		{
			MeshShader->PEffectPass->Apply(0, pImmediateContext);
			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);

			// IMPORTANT pour ajuster les param.
			MeshShader->PEffectPass->Apply(0, pImmediateContext);

			pCB->SetConstantBuffer(MeshShader->PConstantBuffer);
			pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer, 0, nullptr, &sp, 0, 0);

			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}
}
