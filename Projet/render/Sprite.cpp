#include "stdafx.h"
#include "Sprite.h"
#include "MoteurWindows.h"
#include "resources/resource.h"
//#include "util/util.h"
//#include "MoteurWindows.h"

using namespace DirectX;


SpriteVertex::SpriteVertex(const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT2& CoordTex)
	: Position(Position)
	, CoordTex(CoordTex)
{
}

D3D11_INPUT_ELEMENT_DESC SpriteVertex::Layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

UINT SpriteVertex::NumElements = ARRAYSIZE(Layout);

SpriteVertex ShaderSprite::sommets[6] =
{
	SpriteVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
	SpriteVertex(XMFLOAT3(-0.5f, 0.5f, 0.0f), XMFLOAT2(0.0f, 0.0f)),
	SpriteVertex(XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
	SpriteVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT2(0.0f, 1.0f)),
	SpriteVertex(XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT2(1.0f, 0.0f)),
	SpriteVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT2(1.0f, 1.0f))
};




ShaderSprite::ShaderSprite(const wchar_t* FileName) :
	FileName{FileName}
{
	// Création du vertex buffer et copie des sommets
	ID3D11Device* pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(sommets);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = sommets;

	DX_TRY(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

	// Création d'un tampon pour les constantes de l'effet
	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));

	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(ShaderParamsSprite);
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd2.CPUAccessFlags = 0;
	
	pD3DDevice->CreateBuffer(&bd2, nullptr, &pConstantBuffer);

	// Pour l'effet
	ID3DBlob* pFXBlob = nullptr;

	DX_TRY(D3DCompileFromFile(FileName, 0, 0, 0, "fx_5_0", 0, 0, &pFXBlob, 0), DXE_ERREURCREATION_FX);

	D3DX11CreateEffectFromMemory(pFXBlob->GetBufferPointer(), pFXBlob->GetBufferSize(), 0, pD3DDevice, &pEffet);

	pFXBlob->Release();

	pTechnique = pEffet->GetTechniqueByIndex(0);
	pPasse = pTechnique->GetPassByIndex(0);

	// Créer l'organisation des sommets pour le VS de notre effet
	D3DX11_PASS_SHADER_DESC effectVSDesc;
	pPasse->GetVertexShaderDesc(&effectVSDesc);

	D3DX11_EFFECT_SHADER_DESC effectVSDesc2;
	effectVSDesc.pShaderVariable->GetShaderDesc(effectVSDesc.ShaderIndex, &effectVSDesc2);

	const void* vsCodePtr = effectVSDesc2.pBytecode;
	const unsigned vsCodeLen = effectVSDesc2.BytecodeLength;

	DX_TRY(pD3DDevice->CreateInputLayout(
	SpriteVertex::Layout,
		SpriteVertex::NumElements,
		vsCodePtr,
		vsCodeLen,
		&pVertexLayout), DXE_CREATIONLAYOUT);

	// Initialisation des paramètres de sampling de la texture
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Création de l'état de sampling
	pD3DDevice->CreateSamplerState(&samplerDesc, &pSampleState);
}

ShaderSprite::~ShaderSprite()
{
	DX_RELEASE(pConstantBuffer);
	DX_RELEASE(pSampleState);
	DX_RELEASE(pEffet);
	DX_RELEASE(pVertexLayout);
	DX_RELEASE(pVertexBuffer);
}
