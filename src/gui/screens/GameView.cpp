#include <gui/screens/GameView.hpp>
#include <world/World.hpp>

namespace screens {

GameView::GameView() {
	this->world = new World();
}

GameView::~GameView() {
	delete this->world;
}

void GameView::render(double time, double fps, glm::mat4 matrix) {

}

void GameView::onScreenResize() {
}

bool GameView::supportsCursor() {
	return false;
}

} /* namespace screens */
