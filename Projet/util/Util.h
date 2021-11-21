#pragma once

#include <string>

// Physx resources cleaning
#define PX_RELEASE(res) if(res) { res->release(); res = nullptr; }

const wchar_t* str2wchar(const std::string& str) noexcept;
const std::string wchar2str(const wchar_t* wchars) noexcept;

const std::wstring str2wstr(const std::string& str) noexcept;
const std::string wstr2str(const std::wstring& wstr) noexcept;