#ifndef FYP_UTIL_CONSOLE_HPP_
#define FYP_UTIL_CONSOLE_HPP_

#define CONSOLE_TAG_MAIN 0

#include <iostream>
#include <codecvt>

typedef std::codecvt_utf8<wchar_t> convert_type;

class Console {
	public:
		static void println(int index, std::wstring text);
		static void print(int index, std::wstring text);
		static void println(int index, std::string text);
		static void print(int index, std::string text);
};

#endif
