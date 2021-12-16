#pragma once

#include <string>
#include "render/DeviceD3D11.h"
#include "render/d3dx11effect.h"

// Physx resources cleaning
#define PX_RELEASE(res) if(res) { res->release(); res = nullptr; }

// DirectX resources cleaning
template <class Type>
inline void DX_TRY(const Type & Resultat)
{
	if (Resultat != S_OK)
	{
		throw Resultat;
	}
}

template <class Type1, class Type2>
inline void DX_TRY(const Type1 & Resultat, const Type2 & unCode)
{
	if (Resultat != S_OK)
	{
		throw unCode;
	}
}

template <class Type>
inline void DX_VALIDATE(const void* UnPointeur, const Type & unCode)
{
	if (UnPointeur == nullptr)
	{
		throw unCode;
	}
}

template <class Type>
inline void DX_RELEASE(Type & UnPointeur)
{
	if (UnPointeur != nullptr)
	{
		UnPointeur->Release();
		UnPointeur = nullptr;
	}
}

/// <summary>
/// Compile a shader from file.
/// The function take care of version, and handling includes.
/// </summary>
/// <param name="FileName">The path to the shader to compile</param>
/// <param name="PDevice">The device used to load the shader.</param>
/// <returns>The pointer to the created shader.</returns>
ID3DX11Effect* DX_CompileShaderFromFile(const wchar_t* FileName, ID3D11Device* PDevice);

// Assert that a type used for hlsl is correctly alligned
#define DX_HLSL_ASSERT_ALLIGN(type) \
	static_assert(sizeof(type) % 16 == 0, "HLSL require, " #type " to be aligned to 16 bytes.");

// Filler helper to respect allignement
#define DX_HLSL_FILL(i) float __HLSL_FILL__[i] = {};

template <class Type>
inline void DX_RELEASE_INPUT(Type& Pointer)
{
	if (Pointer != nullptr) {
		Pointer->Unacquire();
		Pointer->Release();
		Pointer = nullptr;
	}
}

inline float frand()
{
	return static_cast<float>(rand()) / RAND_MAX;
}

const wchar_t* str2wchar(const std::string& str) noexcept;
const std::string wchar2str(const wchar_t* wchars) noexcept;

const std::wstring str2wstr(const std::string& str) noexcept;
const std::string wstr2str(const std::wstring& wstr) noexcept;

namespace PM3D
{

	// Essayer en envoyant le code d'erreur comme r�sultat
	// Il ne faut pas oublier de "rattraper" le code...
	template <class Type>
	inline void DXEssayer(const Type& Resultat)
	{
		if (Resultat != S_OK)
		{
			throw Resultat;
		}
	}

	// Plus pratique, essayer en envoyant un code sp�cifique 
	// comme r�sultat
	template <class Type1, class Type2>
	inline void DXEssayer(const Type1& Resultat, const Type2& unCode)
	{
		if (Resultat != S_OK)
		{
			throw unCode;
		}
	}

	// Valider un pointeur
	template <class Type>
	inline void DXValider(const void* UnPointeur, const Type& unCode)
	{
		if (UnPointeur == nullptr)
		{
			throw unCode;
		}
	}

	// Rel�cher un objet COM (un objet DirectX dans notre cas)
	template <class Type>
	inline void DXRelacher(Type& UnPointeur)
	{
		if (UnPointeur != nullptr)
		{
			UnPointeur->Release();
			UnPointeur = nullptr;
		}
	}

} // namespace PM3D
