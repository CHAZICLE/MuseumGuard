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

#include <iostream>

#include "GamePauseMenu.hpp"

using namespace screens;

GamePauseMenu::GamePauseMenu() : Screen()
{
	this->resumeButton = new Button("Resume");
	this->pathTestButton = new Button("???");
	this->exitButton = new Button("Quit");

	this->resumeButton->setY(20);
	this->pathTestButton->setY(10);
	//this->exitButton->setY(0);

	this->addElement(this->resumeButton);
	this->addElement(this->pathTestButton);
	this->addElement(this->exitButton);

	this->font = new render::Font("cour.ttf", 22);
}
GamePauseMenu::~GamePauseMenu()
{
	delete this->resumeButton;
	delete this->pathTestButton;
	delete this->exitButton;
}
void GamePauseMenu::render(util::DeltaTime &deltaTime, render::RenderManager &rManager)
{
	super::render(deltaTime, rManager);

	glm::mat4 boxMat = glm::mat4(1.0f);
	double s = (1+std::sin(deltaTime.getTime()*4))/20;
	boxMat = glm::translate(boxMat, glm::vec3(rManager.getWidthMM()/2, rManager.getHeightMM()/2, 0));
	boxMat = glm::scale(boxMat, glm::vec3(1+s, 1+s, 0));
	//boxMat = glm::translate(boxMat, glm::vec3(this->font->getTextWidth("Paused", rManager)/4, 22/2, 0));
	boxMat = glm::translate(boxMat, glm::vec3(-this->font->getTextWidth("Paused", rManager)/4, 0, 0));
	rManager.M = boxMat;
	rManager.markMDirty();
	this->font->printf("Paused", rManager);
}
bool GamePauseMenu::onControlEvent(Control control, int action)
{
	if(action==CONTROL_KEYACTION_RELEASE && control==CONTROL_GUI_ESCAPE)
	{
		this->manager->closeScreen(this);
		return true;
	}
	if((action&CONTROL_MOUSEBUTTONACTION_PRESS) && control==CONTROL_GUI_SELECT)
	{
		if(this->selectedElement==this->resumeButton)
		{
			this->manager->closeScreen(this);
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
	return true;
}
