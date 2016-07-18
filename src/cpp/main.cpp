#include "main.hpp"
#include <fstream>
#include <ctime>

#include "gui/WindowScreenManager.hpp"

std::ostream *debugfile = 0;

int main(int argc, char *argv[])
{
	std::ofstream df;
	df.open("tmp/debug.log", std::ios::out|std::ios::trunc);
	debugfile = dynamic_cast<std::ostream *>(&df);

	std::srand(std::time(0l));

	WindowScreenManager manager;
	manager.run();
}
