#include "gui/elements/Button.hpp"
#include "input/Controls.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screens/PathTest.hpp"
#include "gui/screens/GameView.hpp"

#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/RenderManager.hpp"
#include "util/gl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "MainMenu.hpp"

using namespace screens;

MainMenu::MainMenu() : Screen()
{
	this->playButton = new Button();
	this->pathTestButton = new Button();
	this->exitButton = new Button();

	this->playButton->setY(20);
	this->pathTestButton->setY(10);
	//this->exitButton->setY(0);

	this->addElement(this->playButton);
	this->addElement(this->pathTestButton);
	this->addElement(this->exitButton);
}
MainMenu::~MainMenu()
{
	
}
bool MainMenu::onControlEvent(int control, int action)
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
