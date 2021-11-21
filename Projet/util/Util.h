#pragma once

#include <string>

// Physx resources cleaning
#define PX_RELEASE(res) if(res) { res->release(); res = nullptr; }

const wchar_t* str2whchar(const std::string & str) noexcept;
const std::string whchar2str(const wchar_t* str) noexcept;