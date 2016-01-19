#include "gui/elements/Button.hpp"
#include "input/Controls.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screens/PathTest.hpp"
#include <iostream>

#include "MainMenu.hpp"

MainMenu::MainMenu() : Screen()
{
	this->exitButton = new Button();
	this->addElement(this->exitButton);
}
MainMenu::~MainMenu()
{
	
}
bool MainMenu::onControlEvent(int control, int action)
{
	std::cout << control << ", " << action << std::endl;
	if(action==CONTROL_ACTION_PRESS && control==CONTROL_GUI_SELECT)
	{
		this->manager->openRootScreen(new PathTest());
	}
	return false;
}
