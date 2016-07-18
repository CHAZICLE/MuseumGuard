#include "world/Entity.hpp"
#include "world/World.hpp"
#include "util/Boundaries.hpp"

#include "PerceptionManager.hpp"

using namespace ai;
using namespace world;
using namespace util::Boundaries;

PerceptionManager::PerceptionManager(Entity *controlEntity, std::initializer_list<const std::type_info *> searchTypes, glm::vec3 offset_modelSpace)
{
	this->currentYaw = 0.f;
	this->currentPitch = 0.f;
	this->targetYaw = 0.f;
	this->targetPitch = 0.f;
	this->targetEntity = 0;
	this->offset_modelSpace = offset_modelSpace;
	this->controlEntity = controlEntity;
	this->searchTypes.insert(searchTypes.begin(), searchTypes.end());
	this->minYaw = -M_PI;
	this->maxYaw = M_PI;
}
PerceptionManager::~PerceptionManager()
{
	
}
void PerceptionManager::setYawBounds(float minYaw, float maxYaw)
{
	this->minYaw = minYaw;
	this->maxYaw = maxYaw;
}
extern glm::vec3 debug_point2,debug_point3;
void PerceptionManager::tick(util::DeltaTime &deltaTime)
{
	Raycast ray;
	glm::vec3 targetOrigin = this->controlEntity->getPosition()+(this->controlEntity->getOrientation()*this->offset_modelSpace);
	ray.origin = targetOrigin;
	glm::vec3 targetDirection;
	this->targetEntity = 0;
	debug_point2 = ray.origin;
	Entity *e2;
	for(Entity *e : *this->controlEntity->getWorld().getEntities())
	{
		AABB *bounds = e->getBounds();
		if(bounds!=0 && this->searchTypes.find(&typeid(*e))!=this->searchTypes.end())
		{
			AABB ebounds = bounds->translate(e->getPosition());
			glm::vec3 min = ebounds.min();
			glm::vec3 max = ebounds.max();
			for(int i=0;i<8;i++)
			{
				glm::vec3 tmp = glm::vec3(
						i&1 ? min[0] : max[0],
						i&2 ? min[1] : max[1],
						i&4 ? min[2] : max[2]
					);
				ray.direction = glm::normalize((tmp-ray.origin));
				ray.maxDistance = 0.f;
				e2 = this->controlEntity;
				RaycastResult result = this->controlEntity->getWorld().rayCast(ray, &e2);
				debug_point3 = ray.origin+ray.direction*result.distance;
				if(e2==e)
				{
					PRINT_DEBUG("RAY HIT");
					targetDirection = (((min+max)*0.5f)-ray.origin)*this->controlEntity->getOrientation();
					this->targetEntity = e;
					break;//Stop checking this ent, we can already see it
				}
			}
		}
	}
	if(this->targetEntity!=0)
	{
		this->targetYaw = -std::atan2(targetDirection.x, targetDirection.y);//Rotation around Z
		this->targetPitch = -std::atan2(std::sqrt(targetDirection.x*targetDirection.x+targetDirection.y*targetDirection.y), targetDirection.z)+glm::radians(90.f);
		if(this->targetYaw<minYaw)
			this->targetYaw = minYaw;
		else if(this->targetYaw>maxYaw)
			this->targetYaw = maxYaw;
		if(this->targetPitch<-0.4f)
			this->targetPitch = -0.4f;
	}
#define TURRET_YAW_RESOLUTION 0.05f
	float targetYawRelative = this->targetYaw-this->currentYaw;//Relative to current
	if(targetYawRelative>M_PI)
		targetYawRelative -= M_PI*2.f;
	if(targetYawRelative<-M_PI)
		targetYawRelative += M_PI*2.f;
	if(std::fabs(targetYawRelative)>TURRET_YAW_RESOLUTION)
	{
		this->currentYaw += targetYawRelative>0 ? TURRET_YAW_RESOLUTION : -TURRET_YAW_RESOLUTION;
		if(this->currentYaw>M_PI)
			this->currentYaw -= M_PI*2.f;
		if(this->currentYaw<-M_PI)
			this->currentYaw += M_PI*2.f;
	}
	if(std::fabs(this->currentPitch-this->targetPitch)>0.001f)
	{
		//float dYaw = this->targetYaw-this->currentYaw;
		float dPitch = this->targetPitch-this->currentPitch;
		//this->currentYaw += dYaw/10;
		this->currentPitch += dPitch/10;
	}
}
glm::quat PerceptionManager::getOrientation()
{
	return glm::quat(glm::vec3(this->currentPitch, 0, this->currentYaw));
}
