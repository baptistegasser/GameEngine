#include "stdafx.h"
#include "Util.h"

const wchar_t* str2whchar(const std::string& str) noexcept
{
	return std::wstring{ str.begin(), str.end() }.c_str();
}

const std::string whchar2str(const wchar_t* str) noexcept
{
	const auto wstr = std::wstring(str);
// Ignore this warning we known that converting a wide string to simple string is unsafe, that's the whole purpose of doing it
#pragma warning(suppress: 4244)
	return std::string{ wstr.begin(), wstr.end() };
}
