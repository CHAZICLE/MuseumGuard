#include "world/World.hpp"
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "input/Controls.hpp"

#include "gui/ScreenManager.hpp"
#include "gui/screens/MainMenu.hpp"
#include "gui/screens/GamePauseMenu.hpp"

#include <glm/gtc/matrix_transform.hpp>

//debug
#include <iostream>

#include "GameView.hpp"

using namespace world;
using namespace screens;

GameView::GameView() {
	this->world = new World(this);
	this->statusFont = new render::Font("cour.ttf", 22);
	this->wasSurface = true;
	this->pauseStartTime = 0;
	this->pauseOffsetTime = 0;
	this->gameCompletionState = -1;
}

GameView::~GameView() {
	delete this->world;
}
void GameView::onGameOver(int type)
{
	this->gameCompletionState = type;
}
void GameView::render(util::DeltaTime &deltaTime, render::RenderManager &manager) {
	// Updating the world
	bool isSurface = this->manager->isScreenSurface(this);
	if(this->gameCompletionState>=0)
		isSurface = false;
	if(wasSurface!=isSurface)
	{
		wasSurface = isSurface;
		if(!isSurface)
			pauseStartTime = deltaTime.getTime();
		else
			pauseOffsetTime += deltaTime.getTime()-pauseStartTime;
	}
	util::DeltaTime dt2 = deltaTime;
	dt2.setOffsetTime(-pauseOffsetTime);
	this->world->tick(dt2, isSurface);
	// Render the 3D world
	render::RenderManager gameRenderManager;
	gameRenderManager.setDimensionsPx(manager.getWidthPx(), manager.getHeightPx());
	gameRenderManager.setDimensionsMM(manager.getWidthMM(), manager.getHeightMM());
	gameRenderManager.P = glm::perspective(70.f, (float)(this->manager->getWidth()/this->manager->getHeight()), 0.1f, 10000.f);
	gameRenderManager.markPDirty();
	this->world->render3D(gameRenderManager, isSurface);
	// Render the 2D elements
	this->world->render2D(manager, isSurface);

	if(this->gameCompletionState>=0)
	{
		manager.M = glm::translate(glm::mat4(1.0f), glm::vec3(manager.getWidthMM()/2-this->statusFont->getTextWidth("GAME OVER\0", manager)/4, manager.getHeightMM()/2-22/2, 0));
		manager.markMDirty();
		this->statusFont->printf("GAME OVER", manager);
	}
}
bool GameView::onControlEvent(Control control, int action)
{
	if(control==CONTROL_GUI_ESCAPE && action==CONTROL_KEYACTION_RELEASE)
	{
		this->manager->openScreen(new GamePauseMenu());
		return true;
	}
	if(this->gameCompletionState<0)
		this->world->onDebugControl(control, action);
	return true;
}
void GameView::onScreenResize() {
}
bool GameView::supportsCursor() {
	return false;
}
