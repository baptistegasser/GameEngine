#include "stdafx.h"

#include "Terrain.h"
#include "Vertex.h"
#include "MoteurWindows.h"
#include "math/Math.h"
#include "physic/HeightFieldCollider.h"
#include "resources/resource.h"
#include "physic/RigidBody.h"

using namespace DirectX;

ATerrain::ATerrain(const wchar_t* FileName, XMFLOAT3 Scale, Shader* Shader, const PhysicMaterial& Material, bool BackFaceCulling)
	: Scale{ Scale }
	, MeshShader{ Shader }
	, BackFaceCulling{BackFaceCulling}
{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	std::ifstream bitmapFile(FileName, std::ios::binary);
	if (!bitmapFile) throw;
	bitmapFile.read(reinterpret_cast<char*>(&bitmapFileHeader), sizeof(BITMAPFILEHEADER));
	bitmapFile.read(reinterpret_cast<char*>(&bitmapInfoHeader), sizeof(BITMAPINFOHEADER));

	Height = bitmapInfoHeader.biHeight;
	Width = bitmapInfoHeader.biWidth;
	VertexCount = Height * Width;
	PolyCount = (Height - 1) * (Width - 1) * 2;

	int imageSize;
	const bool extraBit = VertexCount % 2 != 0;
	if (!extraBit) imageSize = Height * (Width * 3);
	else imageSize = Height * ((Width * 3) + 1);

	const auto bitmapImage = new unsigned char [imageSize];
	bitmapFile.read(reinterpret_cast<char*>(bitmapImage), imageSize);

	Vertices = std::vector<vertex_t>(Height * Width);

	int k{ 0 };
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			const int i = (Width * (Height - 1 - z)) + x;

			Vertices[i].Position = XMFLOAT3{
					x * Scale.x,
					bitmapImage[k] * Scale.y,
					z * Scale.z
			};
			Vertices[i].TexCoord = XMFLOAT2{
					x * Scale.x / TextureCoefficient,
					z * Scale.z / TextureCoefficient
			};

			k += 3;
		}
		if (extraBit) k++;
	}

	delete[] bitmapImage;

	ComputeIndexes();
	ComputeNormals();
	GenerateMesh();

	AddComponent<HeightFieldCollider>(Material, this);
	AddComponent<RigidBody>(RigidBody::RigidActorType::Static);
}

ATerrain::~ATerrain()
{
	Actor::~Actor();
	DX_RELEASE(PIndexBuffer);
	DX_RELEASE(PVertexBuffer);
}

void ATerrain::LateTick(const float ElapsedTime)
{
	Actor::LateTick(ElapsedTime);

	matWorld = Transform;

	ID3D11DeviceContext* pImmediateContext = PM3D::CMoteurWindows::GetInstance().GetDispositif().ImmediateContext;

	if (BackFaceCulling)
		PM3D::CMoteurWindows::GetInstance().GetDispositif().DeactivateCullBack();

	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const UINT stride = sizeof(vertex_t);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &PVertexBuffer, &stride, &offset);

	pImmediateContext->IASetIndexBuffer(PIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pImmediateContext->IASetInputLayout(MeshShader->PInputLayout);

	const XMMATRIX& viewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();
	ShaderParams.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	ShaderParams.matWorld = XMMatrixTranspose(matWorld);
	ShaderParams.vLumiere = XMVectorSet(-10.0f, 10.0f, -15.0f, 1.0f);
	ShaderParams.vCamera = XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);
	ShaderParams.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	ShaderParams.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	ShaderParams.vSEcl = XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f);
	ShaderParams.PosScale = XMVectorSet(Scale.x, Scale.z, static_cast<float>(Width), static_cast<float>(Height));
	ShaderParams.TextureCoefficient = TextureCoefficient;
	pImmediateContext->UpdateSubresource(MeshShader->PConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);

	// Textures
	const auto tex1 = MeshShader->PEffect->GetVariableByName("texture1")->AsShaderResource();
	tex1->SetResource(Texture1->TextureView);

	const auto tex2 = MeshShader->PEffect->GetVariableByName("texture2")->AsShaderResource();
	tex2->SetResource(Texture2->TextureView);

	const auto tex3 = MeshShader->PEffect->GetVariableByName("texture3")->AsShaderResource();
	tex3->SetResource(Texture3->TextureView);

	ID3DX11EffectConstantBuffer* pCB = MeshShader->PEffect->GetConstantBufferByName("param");
	pCB->SetConstantBuffer(MeshShader->PConstantBuffer);

	const auto variableSampler = MeshShader->PEffect->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, MeshShader->PSampleState);

	// Render
	MeshShader->PEffectPass->Apply(0, pImmediateContext);
	pImmediateContext->DrawIndexed(static_cast<UINT>(PolyCount * 3), 0, 0);

	if (BackFaceCulling)
		PM3D::CMoteurWindows::GetInstance().GetDispositif().ActivateCullBack();
}

void ATerrain::ComputeNormal(int x, int z)
{
	XMVECTOR n1, n2, n3, n4;
	XMVECTOR v1, v2, v3, v4;

	n1 = n2 = n3 = n4 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	const auto pos = GetVertexPositionVector(x, z);

	if (z < Width - 1)	v1 = GetVertexPositionVector(x, z + 1) - pos;
	if (x < Height - 1) v2 = GetVertexPositionVector(x + 1, z) - pos;
	if (z > 0)			v3 = GetVertexPositionVector(x, z - 1) - pos;
	if (x > 0)			v4 = GetVertexPositionVector(x - 1, z) - pos;

	if (z < Width - 1 && x < Height - 1) n1 = XMVector3Cross(v2, v1);
	if (z > 0 && x < Height - 1)		 n2 = XMVector3Cross(v3, v2);
	if (z > 0 && x > 0)					 n3 = XMVector3Cross(v4, v3);
	if (z < Width - 1 && x > 0)			 n4 = XMVector3Cross(v1, v4);

	n1 = n1 + n2 + n3 + n4;
	n1 = -XMVector3Normalize(n1);
	XMFLOAT3 result{};
	XMStoreFloat3(&result, n1);

	GetVertex(x, z).Normal = result;
}

void ATerrain::ComputeNormals()
{
	for (int z = 0; z < Height; z++)
		for (int x = 0; x < Width; x++)
			ComputeNormal(x, z);
}

void ATerrain::GenerateMesh()
{
	ID3D11Device* PD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().D3DDevice;

	// Vertices
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = static_cast<UINT>(Vertices.size() * sizeof(vertex_t));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Vertices.data();
		PVertexBuffer = nullptr;

		DX_TRY(PD3DDevice->CreateBuffer(&bd, &InitData, &PVertexBuffer), DXE_CREATIONVERTEXBUFFER);
	}

	// Indexes
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = static_cast<UINT>(Indexes.size() * sizeof(index_t));
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = Indexes.data();
		PIndexBuffer = nullptr;

		DX_TRY(PD3DDevice->CreateBuffer(&bd, &InitData, &PIndexBuffer), DXE_CREATIONINDEXBUFFER);
	}
}

void ATerrain::ComputeIndexes()
{
	Indexes = std::vector<index_t>(PolyCount * 3);

	int i{ 0 };

	for (int z = 0; z < Height - 1; ++z)
		for (int x = 0; x < Width - 1; ++x)
		{
			Indexes[i++] = z * Width + x;
			Indexes[i++] = z * Width + (x + 1);
			Indexes[i++] = (z + 1) * Width + (x + 1);

			Indexes[i++] = z * Width + x;
			Indexes[i++] = (z + 1) * Width + (x + 1);
			Indexes[i++] = (z + 1) * Width + x;
		}
}

const ATerrain::vertex_t& ATerrain::GetVertex(int x, int z) const
{
	return Vertices[Width * (Height - 1 - z) + x];
}

ATerrain::vertex_t& ATerrain::GetVertex(int x, int z)
{
	return Vertices[Width * (Height - 1 - z) + x];
}

DirectX::XMVECTOR ATerrain::GetVertexPositionVector(int x, int z)
{
	return XMLoadFloat3(&GetVertex(x, z).Position);
}
