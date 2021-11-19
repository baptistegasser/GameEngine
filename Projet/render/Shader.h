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
	DirectX::XMVECTOR vLumiere; 		// la position de la source d'éclairage (Point)
	DirectX::XMVECTOR vCamera; 			// la position de la caméra
	DirectX::XMVECTOR vAEcl; 			// la valeur ambiante de l'éclairage
	DirectX::XMVECTOR vAMat; 			// la valeur ambiante du matériau
	DirectX::XMVECTOR vDEcl; 			// la valeur diffuse de l'éclairage 
	DirectX::XMVECTOR vDMat; 			// la valeur diffuse du matériau 
	DirectX::XMVECTOR vSEcl; 			// la valeur spéculaire de l'éclairage 
	DirectX::XMVECTOR vSMat; 			// la valeur spéculaire du matériau 
};

class DefaultShader : Shader<DefaultShaderParams> {
public:
	DefaultShader()
		: Shader<DefaultShaderParams>{ L"MiniPhong.fx" }
	{}

	void Init() override;
	DefaultShaderParams GetShaderParams() noexcept override;
};