#pragma once

#include "core/Component.h"

template <class ShaderParams>
class Shader : public Pitbull::Component{
	const wchar_t* FileName;

public:
	explicit Shader(const wchar_t* FileName)
		: FileName{ FileName }
	{}
	// Force shader implementations to create a destructor to hopefully clean their ressources
	virtual ~Shader();

	virtual ShaderParams GetShaderParams() noexcept = 0;
};

template <class ShaderParams>
Shader<ShaderParams>::~Shader() = default;


struct DefaultShaderParams
{
	DirectX::XMMATRIX matWorldViewProj;	// la matrice totale 
	DirectX::XMMATRIX matWorld;			// matrice de transformation dans le monde 
	DirectX::XMVECTOR vLumiere; 		// la position de la source d'�clairage (Point)
	DirectX::XMVECTOR vCamera; 			// la position de la cam�ra
	DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'�clairage
	DirectX::XMVECTOR vAMat; 			// la valeur ambiante du mat�riau
	DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'�clairage 
	DirectX::XMVECTOR vDMat; 			// la valeur diffuse du mat�riau 
	DirectX::XMVECTOR vSEcl; 			// la valeur sp�culaire de l'�clairage 
	DirectX::XMVECTOR vSMat; 			// la valeur sp�culaire du mat�riau 
};

class DefaultShader : Shader<DefaultShaderParams> {
public:
	DefaultShader()
		: Shader<DefaultShaderParams>{ L"MiniPhong.fx" }
	{}

	void Init() override;
	DefaultShaderParams GetShaderParams() noexcept override;
};