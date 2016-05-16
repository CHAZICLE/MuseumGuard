#include "Enemy.hpp"

#include "util/gl.h"
#include "render/RenderManager.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/BasicShapes.hpp"
#include <iostream>

using namespace entities;
using namespace render;

Enemy::Enemy()
{
	
}
Enemy::~Enemy()
{
	
}
void Enemy::render(render::RenderManager &rManager)
{
	// Render cube
	glUseProgram(shaders::program_modelTest);
	rManager.M = glm::scale(glm::translate(glm::mat4(1.0f), this->getPosition()), glm::vec3(1,1,1));
	rManager.markMDirty();
	rManager.setMVPMatrix(shaders::program_modelTest_MVP);
	BasicShapes::renderUnitCube(shaders::program_modelTest_vertexPosition);
}
