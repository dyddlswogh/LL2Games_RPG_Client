#pragma once
#include <string>

namespace utils
{
	inline std::wstring StringToWString(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}
}