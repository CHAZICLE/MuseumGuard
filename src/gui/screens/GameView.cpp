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

namespace screens {

GameView::GameView() {
	this->world = new World();
}

GameView::~GameView() {
	delete this->world;
}
void GameView::render(util::DeltaTime &deltaTime, render::RenderManager &manager) {
	render::RenderManager gameRenderManager;
	gameRenderManager.P = glm::perspective(70.f, (float)(this->manager->getWidth()/this->manager->getHeight()), 1.f, 10000.f);
	gameRenderManager.markPDirty();
	this->world->tick(deltaTime, this->manager->isScreenSurface(this));
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
