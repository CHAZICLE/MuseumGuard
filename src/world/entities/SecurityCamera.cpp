#include <cmath>
#include <glm/gtx/quaternion.hpp>

#include "SecurityCamera.hpp"

using namespace world;
using namespace entities;
using namespace render;

SecurityCamera::SecurityCamera() : Entity::Entity()
{
}
SecurityCamera::~SecurityCamera()
{
	
}
void SecurityCamera::setTarget(glm::vec3 target)
{
	glm::vec3 direction = target-this->getPosition();
	glm::vec3 relativeUpVector = glm::vec3(0, 0, 1);
	glm::quat q = glm::rotation(direction, relativeUpVector);
}
void SecurityCamera::render(RenderManager &rManager)
{
}
