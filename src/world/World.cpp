#include <iostream>
#include "util/gl.h"
#include <glm/gtc/matrix_transform.hpp>

#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/RenderManager.hpp"
#include "render/OBJModel.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

#include "util/DeltaTime.hpp"
#include "util/AssetManager.hpp"

#include "input/controls/DebugControls.hpp"

#include "world/entities/Enemy.hpp"
#include "world/entities/Player.hpp"
#include "world/entities/SecurityCamera.hpp"

#include "World.hpp"

using namespace world;
using namespace entities;
using namespace render;

World::World()
{
	this->player = new Player();
	this->enemy = new Enemy();
	this->add(this->player, glm::vec3(10,1,10));
	this->add(this->enemy, glm::vec3(0,-10,0));

	this->vertAngle = 0;
	this->horizAngle = 0;
	this->lastX = 0;
	this->lastY = 0;
	this->controlScheme = new controls::DebugControls(this->player);

	this->camera = new SecurityCamera();
	this->camera->keepLookingAt(this->enemy);
	this->add(this->camera, glm::vec3(20,0,5));
}
World::~World()
{
	
}
void World::add(Entity *ent, glm::vec3 location)
{
	//ent->world = this;
	ent->setPosition(location);
	this->entities.push_back(ent);
}
void World::tick(util::DeltaTime &deltaTime, bool surface)
{
	if(surface)
	{
		this->enemy->setPosition(glm::vec3(std::sin(deltaTime.getTime()/2)*10, 3, 0));
		this->camera->setPosition(glm::vec3(std::sin(deltaTime.getTime())*4, std::cos(deltaTime.getTime())*4, std::sin(deltaTime.getTime()/2+M_PI/2)*5+10));
		this->camera->setOrientation(glm::quat(glm::vec3(deltaTime.getTime(), 0, 0)));
		this->controlScheme->tick(deltaTime);
		for(Entity *ent : this->entities)
		{
			ent->tick(deltaTime);
		}
	}
	this->viewDirection = this->player->getOrientation()*glm::vec3( 0, 1, 0);
	this->viewUp = this->player->getOrientation()*glm::vec3(0, 0, 1);
	//glm::vec3 a = glm::eulerAngles(this->player->getOrientation());
	//pitch,roll,yaw
	//std::cout << glm::degrees(a.x) << "," << glm::degrees(a.y) << "," << glm::degrees(a.z) << std::endl;
}
void World::render(render::RenderManager &rManager)
{
	rManager.enableDepth();
	rManager.enableCullFace();
	
	rManager.V = glm::lookAt(
			this->player->getPosition(),
			this->player->getPosition() + viewDirection,
			viewUp
		);
	rManager.markVDirty();

	for(Entity *ent : this->entities)
	{
		ent->render(rManager);
	}
	
	/*
	// Render lines
	glUseProgram(shaders::program_solidcolor);
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	rManager.setMVPMatrix(shaders::program_solidcolor_MVP);
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
	}*/

	rManager.M = glm::scale(glm::mat4(1.0f), glm::vec3(1,1,1));
	rManager.markMDirty();

	rManager.disableCullFace();
	((render::OBJModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_3YPWORLD2_OBJ))->render(rManager, SHADER_UVTest);
	rManager.enableCullFace();

	//((render::SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_HELLKNIGHT_MD5MESH))->render(rManager);
	//((render::SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_HELLKNIGHT_MD5MESH))->debugRender(rManager, true, true);
	
	//((render::SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_HELLKNIGHT_MD5MESH))->debugRender(rManager, true, true);

#define RENDER_OBJ(x) do { util::Asset *a = util::AssetManager::getAssetManager()->getAsset(x); if(a!=0) { dynamic_cast<render::OBJModel *>(a)->render(rManager, shaders::program_solidcolor_vertexPosition); } } while(0);

//	util::Asset *a = util::AssetManager::getAssetManager()->getAsset(ASSET_WEIRDSHAPE_OBJ);
//	if(a!=0)
//		((render::OBJModel *)a)->render(rManager, shaders::program_solidcolor_vertexPosition);

	//RENDER_OBJ(ASSET_WEIRDSHAPE_OBJ);
	//RENDER_OBJ(ASSET_WORLD_OBJ);
	//RENDER_OBJ(ASSET_CUBE_OBJ);
	
	rManager.disableDepth();
	rManager.disableCullFace();

	// Draw axis guides
	//
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);

	glUniform4f(loc, 1.0f, 0.0f, 0.0f, 1.0f);
	BasicShapes::drawLine(glm::vec3(0,0,0), glm::vec3(10, 0, 0), vploc);

	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);
	BasicShapes::drawLine(glm::vec3(0,0,0), glm::vec3( 0,10, 0), vploc);

	glUniform4f(loc, 0.0f, 0.0f, 1.0f, 1.0f);
	BasicShapes::drawLine(glm::vec3(0,0,0), glm::vec3( 0, 0,10), vploc);

	// Draw the player orientation
	glm::vec3 playerDirection = glm::vec3(0, 1, 0)*this->player->getOrientation();
	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);
	BasicShapes::drawLine(glm::vec3(0,0,0), glm::vec3( 0, 0, 0)+playerDirection, vploc);

	glUniform4f(loc, 0.0f, 0.0f, 1.0f, 1.0f);
	BasicShapes::drawLine(playerDirection, playerDirection+glm::vec3(0, 0, 0.2)*this->player->getOrientation(), vploc);


	for(auto ent : this->entities)
	{
		rManager.disableDepth();
		ent->renderDebug(rManager, true, true);
		rManager.enableDepth();
	}

}
