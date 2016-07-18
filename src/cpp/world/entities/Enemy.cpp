#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "util/gl.h"
#include "ai/ObjectiveManager.hpp"

#include "render/RenderManager.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"
#include "world/World.hpp"

#include "Enemy.hpp"

using namespace render;
using namespace world;
using namespace world::entities;
using namespace ai;
using namespace ai::path;

Enemy::Enemy(World *world) : super()
{
	this->animTime = 0;
	this->objectiveManager = new ObjectiveManager(world, this, world->world_navigation_graph);
}
Enemy::~Enemy()
{
	
}

void Enemy::tick(util::DeltaTime &deltaTime)
{
	this->animTime = deltaTime.getTime();
	this->objectiveManager->tick(deltaTime);
}
void Enemy::render(render::RenderManager &rManager)
{
	// Render cube
	/*
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	BasicShapes::renderUnitCube(vploc);
	*/
	rManager.pushMatrixM();

	rManager.M = glm::scale(glm::translate(glm::mat4(1.0f), this->getPosition()), glm::vec3(1,1,1));
	rManager.markMDirty();

	rManager.useShader(SHADER_fuzzyModel);

	SkeletalModel *drone = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_ENEMY_MD5MESH);
	SkeletalAnimation *drone_anim = (SkeletalAnimation *)util::AssetManager::getAssetManager()->getAsset(ASSET_ENEMY_MD5ANIM);

	int frame = drone_anim->getFrame(animTime);
	drone->render(rManager, drone_anim->getFrameSkeleton(frame));
	this->bounds = &drone_anim->getFrameBounds(frame);
	//drone_anim->render(rManager, *drone, animTime);

	rManager.popMatrixM();
	//drone_anim->renderBounds(rManager, animTime);
	//
	this->objectiveManager->render(rManager);
}
void Enemy::setPath(std::vector<int> path)
{
}
