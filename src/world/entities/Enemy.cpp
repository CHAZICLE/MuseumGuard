#include "Enemy.hpp"

#include "util/gl.h"
#include "render/RenderManager.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"

#include "render/MD5Model.hpp"
#include "render/MD5AnimatedModel.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace world;
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
	/*
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	BasicShapes::renderUnitCube(vploc);
	*/

	rManager.M = glm::scale(glm::translate(glm::mat4(1.0f), this->getPosition()), glm::vec3(1,1,1));
	rManager.markMDirty();

	rManager.useShader(SHADER_fuzzyModel);

	MD5Model *drone = (MD5Model *)util::AssetManager::getAssetManager()->getAsset(ASSET_DRONE_MK2_MD5MESH);
	MD5AnimatedModel *drone_anim = (MD5AnimatedModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_DRONE_MK2_MD5ANIM);

	drone_anim->render(rManager, *drone, glfwGetTime());
}
