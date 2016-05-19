#include "Globals.hpp"

using namespace util;

void Globals::fatalError(std::string errorMsg)
{
	std::cerr << "FATAL ERROR!" << std::endl;
	std::cerr << errorMsg << std::endl;
	std::exit(1);
}
