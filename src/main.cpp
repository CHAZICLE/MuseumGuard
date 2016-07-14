#include "main.hpp"
#include <fstream>

#include "gui/WindowScreenManager.hpp"

std::ostream *debugfile = 0;

int main(int argc, char *argv[])
{
	std::ofstream df;
	df.open("debug.log", std::ios::out|std::ios::trunc);
	debugfile = dynamic_cast<std::ostream *>(&df);

	WindowScreenManager manager;
	manager.run();
}
