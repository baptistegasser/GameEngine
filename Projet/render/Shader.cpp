#include "stdafx.h"
#include "Shader.h"

#include "MoteurWindows.h"

void DefaultShader::Init()
{
	// TODO use scene to get light and current camera
	// PM3D::CMoteurWindows::GetInstance().GetScene();
}

DefaultShaderParams DefaultShader::GetShaderParams() noexcept
{
	// Get Matrix
	// Get Material(s)
	// Get Ligths
	DefaultShaderParams Params{};
	Params.matWorldViewProj = XMMatrixTranspose(matWorld * viewProj);
	Params.matWorld = XMMatrixTranspose(matWorld);

	Params.vLumiere = XMVectorSet(-10.0f, 10.0f, -15.0f, 1.0f);
	Params.vCamera = XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f);
	Params.vAEcl = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
	Params.vAMat = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	Params.vDEcl = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	Params.vDMat = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	return Params;
}
