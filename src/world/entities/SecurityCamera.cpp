#include <cmath>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "util/gl.h"

#include "render/SkeletalAnimation.hpp"

#include "SecurityCamera.hpp"

using namespace world;
using namespace entities;
using namespace render;

SecurityCamera::SecurityCamera() : super()
{
	this->targetEntity = 0;
	this->currentYaw = 0;
	this->currentPitch = 0;
	this->targetYaw = 0;
	this->targetPitch = 0;
	this->model = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5MESH);
}
SecurityCamera::~SecurityCamera()
{
	
}
void SecurityCamera::keepLookingAt(Entity *ent)
{
	this->targetEntity = ent;
}

#define TRACKING_SPEED 10

void SecurityCamera::tick(util::DeltaTime &deltaTime)
{
		//this->setOrientation(this->getOrientation()*glm::quat(glm::vec3(0, 0, glm::radians(1.f))));
	if(this->targetEntity!=0)
	{
		this->targetPosition = this->targetEntity->getPosition();
		glm::vec3 direction = (this->targetPosition-(this->getPosition()+(this->getOrientation()*this->model->bindPoseSkeleton[1].pos)))*this->getOrientation();
		this->targetYaw = -std::atan2(direction.x, direction.y);//Rotation around Z
		this->targetPitch = -std::atan2(std::sqrt(direction.x*direction.x+direction.y*direction.y), direction.z)+glm::radians(90.f);
	}

	if(this->targetYaw<-M_PI/2)
		this->targetYaw = -M_PI/2;
	else if(this->targetYaw>M_PI/2)
		this->targetYaw = M_PI/2;

}
void SecurityCamera::render(RenderManager &rManager)
{
	if(this->currentYaw!=this->targetYaw || this->currentPitch!=this->targetPitch)
	{
		float dYaw = this->targetYaw-this->currentYaw;
		float dPitch = this->targetPitch-this->currentPitch;
		this->currentYaw += dYaw/10;
		this->currentPitch += dPitch/10;
	}
	//this->setOrientation(this->getOrientation()*glm::quat(glm::vec3(0, 0, glm::radians((float)(std::rand()%100)/10))));
	

	rManager.pushMatrixM();
	rManager.M = glm::translate(rManager.M, this->getPosition())*glm::toMat4(this->getOrientation());
	rManager.markMDirty();
	rManager.useShader(SHADER_fuzzyModel);

	Skeleton skel = this->model->bindPoseSkeleton;
	skel[1].ori = glm::quat(glm::vec3(this->currentPitch, 0, this->currentYaw));//, glm::vec3(0, 0, 1))*glm::angleAxis(this->pitch, glm::vec3(1,0,0));
	this->model->render(rManager, skel);
	//drone->renderSkeleton(rManager, skel);
	//drone->renderWeights(rManager, skel);
	rManager.popMatrixM();
}

//controllable by player
//	update quat
//controllable by scripts
//	lookAt(vector)
//	keepLookingAt(Entity*)
