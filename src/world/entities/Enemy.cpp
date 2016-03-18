#include "Enemy.hpp"

#include "util/gl.h"
#include "render/RenderManager.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/BasicShapes.hpp"
#include <iostream>

using namespace entities;

Enemy::Enemy()
{
	
}
Enemy::~Enemy()
{
	
}
void Enemy::render(render::RenderManager *manager)
{
	// Render cube
	glUseProgram(shaders::program_modelTest);
	manager->M = glm::scale(glm::translate(glm::mat4(1.0f), this->getPosition()), glm::vec3(1,1,1));
	manager->markMDirty();
	manager->setMVPMatrix(shaders::program_modelTest_MVP);
	BasicShapes::renderUnitCube(shaders::program_modelTest_vertexPosition);
}
