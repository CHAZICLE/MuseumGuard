#include <world/World.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"

#include "GameView.hpp"

namespace screens {

GameView::GameView() {
	this->world = new World();
}

GameView::~GameView() {
	delete this->world;
}

void GameView::render(util::DeltaTime *deltaTime, render::RenderManager *manager) {

}

void GameView::onScreenResize() {
}

bool GameView::supportsCursor() {
	return false;
}

} /* namespace screens */
