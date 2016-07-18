#include <cmath>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "util/gl.h"

#include "render/SkeletalAnimation.hpp"

#include "world/entities/Enemy.hpp"

#include "res/md5/security_camera.md5mesh.h"

#include "SecurityCamera.hpp"

using namespace world;
using namespace entities;
using namespace render;
using namespace ai;

SecurityCamera::SecurityCamera() : super()
{
	this->model = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5MESH);
	this->perception = new PerceptionManager(this, {&typeid(Enemy)}, this->model->bindPoseSkeleton[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_CAMERASTALK].pos);
	this->perception->setYawBounds(-M_PI/2, M_PI/2);
	this->initAnimation = (SkeletalAnimation *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5ANIM);
	this->animationDuration = this->initAnimation->getAnimationDuration();
	this->animationCurrent = 0;
	this->initAnimating = true;
	this->selector = false;
	this->bounds = &this->initAnimation->getFrameBounds(-1);
}
SecurityCamera::~SecurityCamera()
{
	
}

#define TRACKING_SPEED 10

void SecurityCamera::tick(util::DeltaTime &deltaTime)
{
	if(this->selector)
		return;
	if(this->initAnimating)
	{
		if(deltaTime.getTime()-this->getSpawnTime()<=this->animationDuration)
		{
			this->animationCurrent = deltaTime.getTime()-this->getSpawnTime();
			this->bounds = &this->initAnimation->getFrameBounds(this->initAnimation->getFrame(this->animationCurrent));
		}
		else
		{
			this->initAnimating = false;
			this->bounds = &this->initAnimation->getFrameBounds(-1);
		}
	}
	else
	{
		this->perception->tick(deltaTime);
	}
}
void SecurityCamera::render(RenderManager &rManager)
{
	//this->setOrientation(this->getOrientation()*glm::quat(glm::vec3(0, 0, glm::radians((float)(std::rand()%100)/10))));
	
	
	rManager.pushMatrixM();
	rManager.M = glm::translate(rManager.M, this->getPosition())*glm::toMat4(this->getOrientation());
	rManager.markMDirty();
	rManager.useShader(SHADER_fuzzyModel);
	
	/*Skeleton skel = this->model->bindPoseSkeleton;
	this->model->render(rManager, skel);*/
	//std::cout << "R time=" << this->animTime << std::endl;
	//this->initAnimation->render(rManager, *this->model, this->animTime);
	//drone->renderSkeleton(rManager, skel);
	//drone->renderWeights(rManager, skel);
	if(this->selector)
	{
		this->model->render(rManager, this->initAnimation->getSkeleton(0));
	}
	else if(this->initAnimating)
	{
		this->model->render(rManager, this->initAnimation->getSkeleton(this->animationCurrent));
		//this->initAnimation->renderBounds(rManager, this->animationCurrent);
	}
	else
	{
		Skeleton skel = this->model->bindPoseSkeleton;
		skel[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_CAMERASTALK].ori = this->perception->getOrientation();
		this->model->render(rManager, skel);
		//this->initAnimation->renderBounds(rManager, this->animationCurrent);
	}
	//this->getBounds()->render(rManager, glm::vec4(1.f, 1.f, 0.f, 1.f), false);
	rManager.popMatrixM();
}

//controllable by player
//	update quat
//controllable by scripts
//	lookAt(vector)
//	keepLookingAt(Entity*)
