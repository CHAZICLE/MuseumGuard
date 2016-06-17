#include <world/World.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "input/Controls.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screens/MainMenu.hpp"
#include "gui/screens/GamePauseMenu.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "GameView.hpp"

using namespace world;

namespace screens {

GameView::GameView() {
	this->world = new World();
	this->wasSurface = true;
	this->pauseStartTime = 0;
	this->pauseOffsetTime = 0;
}

GameView::~GameView() {
	delete this->world;
}
void GameView::render(util::DeltaTime &deltaTime, render::RenderManager &manager) {
	render::RenderManager gameRenderManager;
	gameRenderManager.P = glm::perspective(70.f, (float)(this->manager->getWidth()/this->manager->getHeight()), 0.1f, 10000.f);
	gameRenderManager.markPDirty();
	bool isSurface = this->manager->isScreenSurface(this);
	if(wasSurface!=isSurface)
	{
		wasSurface = isSurface;
		if(!isSurface)
		{
			pauseStartTime = deltaTime.getTime();
			std::cout << "Paused at " << pauseStartTime << std::endl;
		}
		else
		{
			pauseOffsetTime += deltaTime.getTime()-pauseStartTime;
			std::cout << "Resumed at " << deltaTime.getTime() << " (Offset now at " << pauseOffsetTime << ")" << std::endl;
		}
	}
	util::DeltaTime dt2 = deltaTime;
	dt2.setOffsetTime(-pauseOffsetTime);
	this->world->tick(dt2, isSurface);
	this->world->render(gameRenderManager);
}
bool GameView::onControlEvent(Control control, int action)
{
	if(control==CONTROL_GUI_ESCAPE && action==CONTROL_KEYACTION_RELEASE)
	{
		this->manager->openScreen(new GamePauseMenu());
		return true;
	}
	return false;
}
void GameView::onScreenResize() {
}

bool GameView::supportsCursor() {
	return false;
}

}
