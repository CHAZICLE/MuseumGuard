#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "util/gl.h"

#include "render/RenderManager.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

#include "Enemy.hpp"

using namespace render;
using namespace world;
using namespace world::entities;
using namespace ai;
using namespace ai::path;

Enemy::Enemy() : super()
{
	this->animTime = 0;
	this->pathExecuter = new PathExecuter(this);
	this->objectiveManager = new ObjectiveManager(0, this, this->pathExecuter);
}
Enemy::~Enemy()
{
	
}

void Enemy::tick(util::DeltaTime &deltaTime)
{
	this->animTime = deltaTime.getTime();
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

	SkeletalModel *drone = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_DRONE_MK2_MD5MESH);
	SkeletalAnimation *drone_anim = (SkeletalAnimation *)util::AssetManager::getAssetManager()->getAsset(ASSET_DRONE_MK2_MD5ANIM);

	//drone_anim->render(rManager, *drone, animTime);
	drone->render(rManager, drone_anim->getSkeleton(animTime));
	drone_anim->renderBounds(rManager, animTime);

	rManager.popMatrixM();
}
