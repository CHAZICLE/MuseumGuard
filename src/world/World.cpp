#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "util/gl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include <iostream>
#include "input/controls/DebugControls.hpp"

#include "World.hpp"

using namespace entities;

World::World()
{
	this->player = new Player();
	this->player->setPosition(glm::vec3(100,10,100));
	this->enemy = new Enemy();
	this->enemy->setPosition(glm::vec3(100,0,0));
	this->vertAngle = 0;
	this->horizAngle = 0;
	this->lastX = 0;
	this->lastY = 0;
	this->controlScheme = new controls::DebugControls(this->player);
}
World::~World()
{
	
}
void World::tick(util::DeltaTime *deltaTime)
{
	this->viewDirection = glm::vec3( 0, 0,-1)*this->player->getOrientation();
	this->viewUp = glm::vec3(0, 1, 0)*this->player->getOrientation();
	this->controlScheme->tick(deltaTime);
}
void World::render(render::RenderManager *manager)
{
	manager->enableDepth();
	manager->enableCullFace();
	
	manager->V = glm::lookAt(
			this->player->getPosition(),
			this->player->getPosition() + viewDirection,
			viewUp
		);
	manager->markVDirty();
	this->player->render(manager);
	this->enemy->render(manager);
	
	// Render lines
	glUseProgram(shaders::program_solidcolor);
	manager->M = glm::mat4(1.0f);
	manager->markMDirty();
	manager->setMVPMatrix(shaders::program_solidcolor_MVP);
	glUniform4f(shaders::program_solidcolor_inColor, 1.0f, 0.0f, 0.0f, 1.0f);
	for(int x=-50;x<=50;x+=10)
	{
		for(int y=-50;y<=50;y+=10)
		{
			if(x!=50)
				BasicShapes::drawLine(glm::vec3(x,0,y), glm::vec3(x+10,0,y+00), shaders::program_solidcolor_vertexPosition);
			if(y!=50)
				BasicShapes::drawLine(glm::vec3(x,0,y), glm::vec3(x+00,0,y+10), shaders::program_solidcolor_vertexPosition);
			if(x!=50 && y!=50)
				BasicShapes::drawLine(glm::vec3(x,0,y), glm::vec3(x+10,0,y+10), shaders::program_solidcolor_vertexPosition);
		}
	}
	
	// Render cube
	glUseProgram(shaders::program_modelTest);
	manager->M = glm::scale(glm::mat4(1.0f), glm::vec3(10,10,10));
	manager->markMDirty();
	manager->setMVPMatrix(shaders::program_modelTest_MVP);
	BasicShapes::renderUnitCube(shaders::program_modelTest_vertexPosition);
	
	manager->disableCullFace();
	manager->disableDepth();
}
