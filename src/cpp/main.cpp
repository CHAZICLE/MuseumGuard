#include "main.hpp"
#include <fstream>
#include <ctime>
#include <csignal>

#include "gui/WindowScreenManager.hpp"

std::ostream *debugfile = 0;

void sigtermHandler(int sig){
	std::cout << "Interrupt" << std::endl;
}

int main(int argc, char *argv[])
{
	std::signal(SIGHUP, sigtermHandler);

	std::ofstream df;
	df.open("tmp/debug.log", std::ios::out|std::ios::trunc);
	debugfile = dynamic_cast<std::ostream *>(&df);

	std::srand(static_cast<unsigned int>(std::time(0l)));

	WindowScreenManager manager;
	manager.run();
}
