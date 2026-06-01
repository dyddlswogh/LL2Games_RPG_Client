#pragma once
#include <charconv>
#include <string_view>
#include <system_error>
#include <string>
#include <Windows.h>


namespace Convert
{
	/*
	string_view??ë¬¸ì?´ì„ ë³µì‚¬?˜ì? ?Šê³  ì°¸ì¡°ë§??˜ëŠ” ê°ì²´?´ë‹¤.
	*/
	inline bool StringToInt(std::string_view s, int& output)
	{
		// string??data ?¨ìˆ˜??ë¬¸ì?´ì˜ ?œì‘ ì£¼ì†Œë¥?ê°€?¸ì˜¨??
		const char* b = s.data();
		const char* e = s.data() + s.size();

		// from_chars??bë¶€??e?„ê¹Œì§€ ë¬¸ìë¥??½ì–´???•ìˆ˜ë¡??´ì„?˜ê³  ê·?ê²°ê³¼ë¥?output???€?¥í•œ??
		// êµ¬ì¡° ë¶„í•´ ë¬¸ë²•?¼ë¡œ??p : ?´ë””ê¹Œì? ?½ì—ˆ?”ì? ê°€ë¦¬í‚¤???¬ì¸?? ec : ?ëŸ¬ ì½”ë“œ
		auto [p, ec] = std::from_chars(b, e, output);
		// ec == std::errc{}???˜ë????ëŸ¬ ?†ìŒ
		return ec == std::errc{} && p == e;
	}

	inline bool StringToInt64(std::string_view s, int64_t& output)
	{
		// string??data ?¨ìˆ˜??ë¬¸ì?´ì˜ ?œì‘ ì£¼ì†Œë¥?ê°€?¸ì˜¨??
		const char* b = s.data();
		const char* e = s.data() + s.size();

		// from_chars??bë¶€??e?„ê¹Œì§€ ë¬¸ìë¥??½ì–´???•ìˆ˜ë¡??´ì„?˜ê³  ê·?ê²°ê³¼ë¥?output???€?¥í•œ??
		// êµ¬ì¡° ë¶„í•´ ë¬¸ë²•?¼ë¡œ??p : ?´ë””ê¹Œì? ?½ì—ˆ?”ì? ê°€ë¦¬í‚¤???¬ì¸?? ec : ?ëŸ¬ ì½”ë“œ
		auto [p, ec] = std::from_chars(b, e, output);
		// ec == std::errc{}???˜ë????ëŸ¬ ?†ìŒ
		return ec == std::errc{} && p == e;
	}

	inline bool StringToFloat(const std::string& s, float& out)
	{
		char* end = nullptr;
		errno = 0;
		out = std::strtof(s.c_str(), &end);
		if (errno != 0) return false;                // ë²”ìœ„ ?¤ë¥˜ ??
		if (end == s.c_str()) return false;          // ë³€?˜ëœ ê²??†ìŒ
		return *end == '\0';                         // ?ê¹Œì§€ ???Œë¹„?ˆëŠ”ì§€
	};

	// wstring(UTF-16) ??string(UTF-8) ë³€??
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

	// string(UTF-8) ??wstring(UTF-16) ë³€??(??°©?? ?„ìš” ???¬ìš©)
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

		// resize¿¡ null ¹®ÀÚ±îÁö Æ÷ÇÔµÇ¾î ÀÖÀ¸¹Ç·Î Á¦°Å
		if (!ansi.empty() && ansi.back() == '\0')
			ansi.pop_back();

		return ansi;
	}

	inline std::wstring StringToWString(const std::string& str)
	{
		if (str.empty())
			return L"";

		int sizeNeeded = MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			-1,
			nullptr,
			0
		);

		if (sizeNeeded <= 0)
			return L"";

		std::wstring result(sizeNeeded - 1, L'\0');

		MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			-1,
			result.data(),
			sizeNeeded
		);

		return result;
	}
};

