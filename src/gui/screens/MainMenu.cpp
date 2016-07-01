#include "gui/ScreenManager.hpp"
#include "gui/screens/PathTest.hpp"
#include "gui/screens/GameView.hpp"
#include "gui/elements/Button.hpp"

#include "MainMenu.hpp"

using namespace screens;

MainMenu::MainMenu() : Screen()
{
	this->playButton = new Button("Play");
	this->pathTestButton = new Button("???");
	this->exitButton = new Button("Quit");

	this->playButton->setY(20);
	this->pathTestButton->setY(10);
	//this->exitButton->setY(0);

	this->addElement(this->playButton);
	this->addElement(this->pathTestButton);
	this->addElement(this->exitButton);
}
MainMenu::~MainMenu()
{
	delete this->playButton;
	delete this->pathTestButton;
	delete this->exitButton;
}
bool MainMenu::onControlEvent(Control control, int action)
{
	if((action&CONTROL_MOUSEBUTTONACTION_PRESS) && control==CONTROL_GUI_SELECT)
	{
		if(this->selectedElement==this->playButton)
		{
			this->manager->openRootScreen(new GameView());
			return true;
		}
		else if(this->selectedElement==this->pathTestButton)
		{
			this->manager->openRootScreen(new PathTest());
			return true;
		}
		else if(this->selectedElement==this->exitButton)
		{
			this->manager->close();
			return true;
		}
	}
	return false;
}
