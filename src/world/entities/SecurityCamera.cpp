#include <cmath>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "util/gl.h"

#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

#include "SecurityCamera.hpp"

using namespace world;
using namespace entities;
using namespace render;

SecurityCamera::SecurityCamera() : Entity::Entity()
{
	this->trackingEntity = 0;
}
SecurityCamera::~SecurityCamera()
{
	
}
//void SecurityCamera::setTarget(glm::vec3 target)
//{
//	glm::vec3 direction = target-this->getPosition();
//	glm::vec3 relativeUpVector = glm::vec3(0, 0, 1);
//	glm::quat q = glm::rotation(direction, relativeUpVector);
//}
void SecurityCamera::keepLookingAt(Entity *ent)
{
	this->trackingEntity = ent;
}
void SecurityCamera::render(RenderManager &rManager)
{
	SkeletalModel *drone = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5MESH);

	Skeleton skel = drone->bindPoseSkeleton;
	//skel[1].pos += glm::vec3(0, 0, 2);


	if(this->trackingEntity!=0)
	{
		glm::vec3 direction = (this->trackingEntity->getPosition()-(this->getPosition()+(this->getOrientation()*skel[1].pos)))*this->getOrientation();
		this->yaw = -std::atan2(direction.x, direction.y);//Rotation around Z
		this->pitch = -std::atan2(std::sqrt(direction.x*direction.x+direction.y*direction.y), direction.z)+glm::radians(90.f);
	}

	this->setOrientation(this->getOrientation()*glm::quat(glm::vec3(0, 0, glm::radians(1.f))));
	//this->pitch += 0.01f;
	//this->yaw += 0.01f;
	if(this->yaw<-M_PI/2)
		this->yaw = -M_PI/2;
	else if(this->yaw>M_PI/2)
		this->yaw = M_PI/2;
	//std::cout << this->pitch*(180/M_PI) << std::endl;

	rManager.M = glm::translate(rManager.M, this->getPosition())*glm::toMat4(this->getOrientation());
	rManager.markMDirty();
	rManager.useShader(SHADER_fuzzyModel);

	//drone->renderWeights(rManager, drone->bindPoseSkeleton);
	SkeletalAnimation *drone_anim = (SkeletalAnimation *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5ANIM);
	
	skel[1].ori = glm::angleAxis(this->yaw, glm::vec3(0, 0, 1))*glm::angleAxis(this->pitch, glm::vec3(1,0,0));
	//drone->renderSkeleton(rManager, skel);
	drone->render(rManager, skel);

	//drone->render(rManager);
	//drone->renderSkeleton(rManager, drone->bindPoseSkeleton);
	//drone_anim->render(rManager, *drone, glfwGetTime());
}

//controllable by player
//	update quat
//controllable by scripts
//	lookAt(vector)
//	keepLookingAt(Entity*)
