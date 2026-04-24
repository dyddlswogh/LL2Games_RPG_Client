#pragma once
#include <charconv>
#include <string_view>
#include <system_error>


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
};

