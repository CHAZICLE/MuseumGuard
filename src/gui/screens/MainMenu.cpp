#include "gui/elements/Button.hpp"
#include "input/Controls.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screens/PathTest.hpp"
#include <iostream>

#include "MainMenu.hpp"

MainMenu::MainMenu() : Screen()
{
	this->playButton = new Button();
	this->exitButton = new Button();

	this->playButton->setY(10);

	this->addElement(this->playButton);
	this->addElement(this->exitButton);
}
MainMenu::~MainMenu()
{
	
}
bool MainMenu::onControlEvent(int control, int action)
{
	std::cout << control << ", " << action << std::endl;
	if((action&CONTROL_MOUSEBUTTONACTION_PRESS) && control==CONTROL_GUI_SELECT)
	{
		if(this->selectedElement==this->playButton)
		{
			this->manager->openRootScreen(new PathTest());
		}
		if(this->selectedElement==this->exitButton)
		{
			this->manager->close();
		}
	}
	return false;
}
