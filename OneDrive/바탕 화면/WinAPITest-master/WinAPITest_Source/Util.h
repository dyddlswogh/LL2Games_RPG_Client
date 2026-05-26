#pragma once
#include <string>

namespace utils
{
	std::wstring StringToWString(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}
}