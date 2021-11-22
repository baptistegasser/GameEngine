#include "stdafx.h"
#include "Util.h"

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
