#include "stdafx.h"
#include "Util.h"

#include "resources/Resource.h"

ID3DX11Effect* DX_CompileShaderFromFile(const wchar_t* FileName, ID3D11Device* PDevice)
{
	ID3DBlob* PFXBlob = nullptr;
	ID3DX11Effect *PEffect = nullptr;
	
	// Compile to memory
	DX_TRY(D3DCompileFromFile(FileName, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, 0, "fx_5_0", 0, 0, &PFXBlob, 0), DXE_ERREURCREATION_FX);
	// Create effect from compiled memory
	D3DX11CreateEffectFromMemory(PFXBlob->GetBufferPointer(), PFXBlob->GetBufferSize(), 0, PDevice, &PEffect);
	
	PFXBlob->Release();

	return PEffect;
}

const wchar_t* str2wchar(const std::string& str) noexcept
{
	return str2wstr(str).c_str();
}

const std::string wchar2str(const wchar_t* whcars) noexcept
{
	return wstr2str(std::wstring(whcars));
}

const std::wstring str2wstr(const std::string& str) noexcept
{
	return std::wstring(str.begin(), str.end());
}

const std::string wstr2str(const std::wstring& wstr) noexcept
{
#pragma warning(suppress: 4244)
	return std::string{ wstr.begin(), wstr.end() };
}
