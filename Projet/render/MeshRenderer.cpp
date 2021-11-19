#include "stdafx.h"
#include "MeshRenderer.h"
#include "MeshLoader.h"



MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, const std::string& MeshFileName)
	: MeshRenderer{ Parent, MeshFileName, Shader{L"Default.fx"}}
{}

MeshRenderer::MeshRenderer(Pitbull::Actor* Parent, const std::string& MeshFileName, Shader& MeshShader)
	: Pitbull::Component{ Parent }
	, MeshShader{ MeshShader }
	, MeshFileName{ MeshFileName }
	, Materials{ Materials }
	, matWorld{ XMMatrixIdentity() }
{}

void MeshRenderer::Init()
{
	OMBMeshLoader Loader;
	Loader.Load(MeshFileName);

	PIndexBuffer = Loader.PIndexBuffer;
	PVertexBuffer = Loader.PVertexBuffer;
	SubsetCount = Loader.SubsetCount;
	SubsetMaterialIndex = Loader.SubsetMaterialIndex;
	SubsetIndex = Loader.SubsetIndex;
	Materials = Loader.Materials;
}

void MeshRenderer::Tick(const float& delta_time)
{
	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(MeshShader.PInputLayout);

	// Index buffer
	pImmediateContext->IASetIndexBuffer(PIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Vertex buffer
	UINT stride = sizeof(PM3D::CSommetBloc);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &PVertexBuffer, &stride, &offset);

	// Initialiser et sélectionner les «constantes» de l'effet
	ShadersParams sp;
	const XMMATRIX& viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();

	sp.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	sp.matWorld = XMMatrixTranspose(matWorld);

	sp.vLumiere = XMVectorSet(-10.0f, 10.0f, -15.0f, 1.0f);
	sp.vCamera = XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);
	sp.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	sp.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	sp.vSEcl = XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = MeshShader.PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader.PSampleState);

	// Dessiner les subsets non-transparents
	for (int32_t i = 0; i < SubsetCount; ++i)
	{
		int32_t indexStart = SubsetIndex[i];
		int32_t indexDrawAmount = SubsetIndex[i + 1] - SubsetIndex[i];
		if (indexDrawAmount)
		{
			sp.vAMat = XMLoadFloat4(&Materials[SubsetMaterialIndex[i]].Ambiante);
			sp.vDMat = XMLoadFloat4(&Materials[SubsetMaterialIndex[i]].Roughness);
			sp.vSMat = XMLoadFloat4(&Materials[SubsetMaterialIndex[i]].Specular);
			sp.puissance = Materials[SubsetMaterialIndex[i]].Power;

			// Activation de la texture ou non
			if (Materials[SubsetMaterialIndex[i]].Texture->GetD3DTexture() != nullptr)
			{
				ID3DX11EffectShaderResourceVariable* variableTexture;
				variableTexture = MeshShader.PEffect->GetVariableByName("textureEntree")->AsShaderResource();
				variableTexture->SetResource(Materials[SubsetMaterialIndex[i]].Texture->GetD3DTexture());
				sp.bTex = 1;
			}
			else
			{
				sp.bTex = 1;
			}

			// IMPORTANT pour ajuster les param.
			MeshShader.PEffectPass->Apply(0, pImmediateContext);

			ID3DX11EffectConstantBuffer* pCB = MeshShader.PEffect->GetConstantBufferByName("param");
			pCB->SetConstantBuffer(MeshShader.PConstantBuffer);
			pImmediateContext->UpdateSubresource(MeshShader.PConstantBuffer, 0, nullptr, &sp, 0, 0);

			pImmediateContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}
}
