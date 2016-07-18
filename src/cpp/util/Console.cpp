#include <locale>

#include "Console.hpp"

void Console::println(int index, std::wstring text)
{
	std::wstring_convert<convert_type, wchar_t> converter;
	std::cout << "[" << std::to_string(index) << "]" << converter.to_bytes(text) << std::endl;
}
void Console::print(int index, std::wstring text)
{
	std::wstring_convert<convert_type, wchar_t> converter;
	std::cout << converter.to_bytes(text);
}
void Console::println(int index, std::string text)
{
	std::cout << "[" << std::to_string(index) << "]" << text << std::endl;
}
void Console::print(int index, std::string text)
{
	std::cout << text;
}
