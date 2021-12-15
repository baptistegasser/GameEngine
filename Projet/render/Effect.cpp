#include "stdafx.h"
#include "Effect.h"

#include "util/Util.h"
#include "resources/resource.h"
#include "MoteurWindows.h"

// Specify vertex layout for DirectX
D3D11_INPUT_ELEMENT_DESC EffectVertex::Layout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
UINT EffectVertex::LayoutSize = ARRAYSIZE(Layout);

// Simple definition of 2 triangles
const EffectVertex Effect::Vertices[6] = {
	EffectVertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	EffectVertex{{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
	EffectVertex{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
	EffectVertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	EffectVertex{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}},
	EffectVertex{{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
};

Effect::~Effect() noexcept
{
	DX_RELEASE(PSampleState);
	DX_RELEASE(PEffet);
	DX_RELEASE(PVertexLayout);
	DX_RELEASE(PVertexBuffer);
	DX_RELEASE(PResourceView);
	DX_RELEASE(PRenderTargetView);
	DX_RELEASE(PTextureScene);
	DX_RELEASE(PDepthStencilView);
	DX_RELEASE(PDepthTexture);
}

Effect::Effect(const wchar_t* FileName)
	: FileName{ FileName }
	, PTechnique{ nullptr }
	, PPass{ nullptr }
	, PVertexLayout{ nullptr }
	, PSampleState{ nullptr }
	, PTextureScene{ nullptr }
	, PRenderTargetView{ nullptr }
	, PResourceView{ nullptr }
	, PDepthTexture{ nullptr }
	, PDepthStencilView{ nullptr }
	, POldRenderTargetView{ nullptr }
	, POldDepthStencilView{ nullptr }
{
	PDevice = &PM3D::CMoteurWindows::GetInstance().GetDispositif();
	ID3D11Device* PD3DDevice = PDevice->D3DDevice;

	D3D11_BUFFER_DESC BuffDesc;
	ZeroMemory(&BuffDesc, sizeof(BuffDesc));
	BuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BuffDesc.ByteWidth = sizeof(Vertices);
	BuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BuffDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = Vertices;
	DX_TRY(PD3DDevice->CreateBuffer(&BuffDesc, &InitData, &PVertexBuffer), DXE_CREATIONVERTEXBUFFER);

	// Compile effect and store technique and pass
	PEffet = DX_CompileShaderFromFile(FileName, PD3DDevice);
	PTechnique = PEffet->GetTechniqueByIndex(0);
	PPass = PTechnique->GetPassByIndex(0);

	// Create vertex layout for vertex shader
	D3DX11_PASS_SHADER_DESC PassDesc;
	D3DX11_EFFECT_SHADER_DESC EffectDesc;
	PPass->GetVertexShaderDesc(&PassDesc);
	PassDesc.pShaderVariable->GetShaderDesc(PassDesc.ShaderIndex, &EffectDesc);

	DX_TRY(PD3DDevice->CreateInputLayout(EffectVertex::Layout,
		EffectVertex::LayoutSize,
		EffectDesc.pBytecode,
		EffectDesc.BytecodeLength,
		&PVertexLayout),
		DXE_CREATIONLAYOUT);

	// Init texture's sampling, simple as it's a unit texture (1.f / 1.f)
	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 0;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	PD3DDevice->CreateSamplerState(&SamplerDesc, &PSampleState);


	D3D11_TEXTURE2D_DESC TextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
	// Config texture to use as render target and resource texture
	TextureDesc.Width = PDevice->ScreenWidth;
	TextureDesc.Height = PDevice->ScreenHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;
	PD3DDevice->CreateTexture2D(&TextureDesc, nullptr, &PTextureScene);
	
	// View for render target
	RenderTargetViewDesc.Format = TextureDesc.Format;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;
	PD3DDevice->CreateRenderTargetView(PTextureScene, &RenderTargetViewDesc, &PRenderTargetView);

	// View for shader resources
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;
	PD3DDevice->CreateShaderResourceView(PTextureScene, &ShaderResourceViewDesc, &PResourceView);

	// Texture stencil
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = PDevice->ScreenWidth;
	depthTextureDesc.Height = PDevice->ScreenHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;
	DX_TRY(PD3DDevice->CreateTexture2D(&depthTextureDesc, NULL, &PDepthTexture), DXE_ERREURCREATIONTEXTURE);
	
	// Stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = depthTextureDesc.Format;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;
	DX_TRY(PD3DDevice->CreateDepthStencilView(PDepthTexture, &DepthStencilViewDesc, &PDepthStencilView), DXE_ERREURCREATIONDEPTHSTENCILTARGET);
}

void Effect::Draw()
{
	ID3D11DeviceContext* pImmediateContext = PDevice->ImmediateContext;

	// Set the vertex data
	UINT stride = sizeof(EffectVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &PVertexBuffer, &stride, &offset);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set layout
	pImmediateContext->IASetInputLayout(PVertexLayout);

	// Set sampler state
	ID3DX11EffectSamplerVariable* SamplerVar;
	SamplerVar = PEffet->GetVariableByName("SampleState")->AsSampler();
	SamplerVar->SetSampler(0, PSampleState);

	// Set texture from previous render
	ID3DX11EffectShaderResourceVariable* TextureVar;
	TextureVar = PEffet->GetVariableByName("Texture")->AsShaderResource();
	TextureVar->SetResource(PResourceView);

	// Apply pass and render
	PPass->Apply(0, pImmediateContext);
	pImmediateContext->Draw(6, 0);
}

void Effect::DebutPostEffect()
{
	// Save current view and stencil
	POldRenderTargetView = PDevice->RenderTargetView;
	POldDepthStencilView = PDevice->DepthStencilView;
	
	// Set texture and stencil as render target
	PDevice->SetRenderTargetView(PRenderTargetView, PDepthStencilView);
}

void Effect::FinPostEffect()
{
	// Restore old target view and stencil
	PDevice->SetRenderTargetView(POldRenderTargetView, POldDepthStencilView);
}