#pragma once
#include <charconv>
#include <string_view>
#include <system_error>
#include <string>
#include <Windows.h>


namespace Convert
{
	/*
	string_view는 문자열을 복사하지 않고 참조만 하는 객체이다.
	*/
	inline bool StringToInt(std::string_view s, int& output)
	{
		// string의 data 함수는 문자열의 시작 주소를 가져온다.
		const char* b = s.data();
		const char* e = s.data() + s.size();

		// from_chars는 b부터 e전까지 문자를 읽어서 정수로 해석하고 그 결과를 output에 저장한다.
		// 구조 분해 문법으로서 p : 어디까지 읽었는지 가리키는 포인터, ec : 에러 코드
		auto [p, ec] = std::from_chars(b, e, output);
		// ec == std::errc{}의 의미는 에러 없음
		return ec == std::errc{} && p == e;
	}

	inline bool StringToFloat(const std::string& s, float& out)
	{
		char* end = nullptr;
		errno = 0;
		out = std::strtof(s.c_str(), &end);
		if (errno != 0) return false;                // 범위 오류 등
		if (end == s.c_str()) return false;          // 변환된 게 없음
		return *end == '\0';                         // 끝까지 다 소비했는지
	};

	// wstring(UTF-16) → string(UTF-8) 변환
	inline std::string WstrToUtf8(const std::wstring& wstr)
	{
		if (wstr.empty())
			return {};

		int utf8Len = WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.c_str(), (int)wstr.size(),
			nullptr, 0,
			nullptr, nullptr);

		if (utf8Len <= 0)
			return {};

		std::string result(utf8Len, '\0');
		WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.c_str(), (int)wstr.size(),
			&result[0], utf8Len,
			nullptr, nullptr);

		return result;
	}

	// string(UTF-8) → wstring(UTF-16) 변환 (역방향, 필요 시 사용)
	inline std::wstring Utf8ToWstr(const std::string& str)
	{
		if (str.empty())
			return {};

		int wideLen = MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), (int)str.size(),
			nullptr, 0);

		if (wideLen <= 0)
			return {};

		std::wstring result(wideLen, L'\0');
		MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), (int)str.size(),
			&result[0], wideLen);

		return result;
	}


	inline std::string Utf8ToAnsi(const std::string& utf8)
	{
		if (utf8.empty())
			return std::string();

		// 1. UTF-8 -> UTF-16
		int wideLen = MultiByteToWideChar(
			CP_UTF8,
			0,
			utf8.c_str(),
			-1,
			nullptr,
			0
		);

		if (wideLen <= 0)
			return std::string();

		std::wstring wide;
		wide.resize(wideLen);

		int wideResult = MultiByteToWideChar(
			CP_UTF8,
			0,
			utf8.c_str(),
			-1,
			&wide[0],
			wideLen
		);

		if (wideResult <= 0)
			return std::string();

		// 2. UTF-16 -> CP949
		int ansiLen = WideCharToMultiByte(
			949,    // CP949
			0,
			wide.c_str(),
			-1,
			nullptr,
			0,
			nullptr,
			nullptr
		);

		if (ansiLen <= 0)
			return std::string();

		std::string ansi;
		ansi.resize(ansiLen);

		int ansiResult = WideCharToMultiByte(
			949,    // CP949
			0,
			wide.c_str(),
			-1,
			&ansi[0],
			ansiLen,
			nullptr,
			nullptr
		);

		if (ansiResult <= 0)
			return std::string();

		// resize에 null 문자까지 포함되어 있으므로 제거
		if (!ansi.empty() && ansi.back() == '\0')
			ansi.pop_back();

		return ansi;
	}
};

