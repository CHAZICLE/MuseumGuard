#include <world/World.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "input/Controls.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screens/MainMenu.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "GameView.hpp"


namespace screens {

GameView::GameView() {
	this->world = new World();
}

GameView::~GameView() {
	delete this->world;
}
void GameView::render(util::DeltaTime *deltaTime, render::RenderManager *manager) {
	manager = new render::RenderManager();
	manager->P = glm::perspective(70.f, (float)(this->manager->getWidth()/this->manager->getHeight()), 1.f, 10000.f);
	manager->markPDirty();
	this->world->tick(deltaTime);
	this->world->render(manager);
}
bool GameView::onControlEvent(int control, int action)
{
	if(control==CONTROL_GUI_ESCAPE && (action&CONTROL_KEYACTION_PRESS))
	{
		this->manager->openRootScreen(new MainMenu());
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
