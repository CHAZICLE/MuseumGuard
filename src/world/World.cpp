#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "util/gl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "World.hpp"

using namespace entities;

World::World() {
	this->player = new Player();
	this->enemy = new Enemy();
	this->vertAngle = 0;
	this->horizAngle = 0;
	this->lastX = 0;
	this->lastY = 0;
}
World::~World() {

}
void World::tick(util::DeltaTime *deltaTime) {
	// Basic mouse controls
	GLFWwindow *window = glfwGetCurrentContext();
	double x, y, dx, dy;
	glfwGetCursorPos(window, &x, &y);
	dx = this->lastX - x;
	dy = this->lastY - y;

	this->horizAngle += dx;
	this->vertAngle += dy;

	this->viewDirection = glm::vec3(
			cos(this->vertAngle) * sin(this->horizAngle), sin(this->vertAngle),
			cos(this->vertAngle) * cos(this->horizAngle));
	glm::vec3 viewRight = glm::vec3(sin(this->horizAngle) + 3.14f / 2, 0,
			cos(this->horizAngle + 3.14f / 2));
	this->viewUp = glm::cross(this->viewDirection, viewRight);
}
void World::render(render::RenderManager *manager) {
	glm::lookAt(this->player->getPosition(),
			this->player->getPosition() + viewDirection, viewUp);
	this->player->render(manager);
	this->enemy->render(manager);
}
