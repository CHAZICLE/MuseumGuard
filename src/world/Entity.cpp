#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"

//debug
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Entity.hpp"

using namespace world;
using namespace util::Boundaries;

Entity::Entity()
{
	this->bounds = 0;
	this->doRender = true;
}
Entity::~Entity()
{
	
}

void Entity::addedToWorld(world::World *world, double spawnTime)
{
	this->world = world;
	this->spawnTime = spawnTime;
}
World &Entity::getWorld()
{
	return *this->world;
}
double Entity::getSpawnTime()
{
	return this->spawnTime;
}
glm::vec3 Entity::getPosition()
{
	return this->position;
}
void Entity::setPosition(glm::vec3 location)
{
	this->position = location;
}
void Entity::translate(glm::vec3 offset)
{
	this->position += offset;
}
glm::quat Entity::getOrientation()
{
	return this->orientation;
}
void Entity::setOrientation(glm::quat rotation)
{
	this->orientation = rotation;
}
AABB *Entity::getBounds()
{
	return this->bounds;
}
void Entity::tick(util::DeltaTime &deltaTime)
{
}
void Entity::render(render::RenderManager &rManager)
{
	
}
void Entity::renderDebug(render::RenderManager &rManager, bool renderPositionMarker, bool renderBounds)
{
	if(renderPositionMarker)
		rManager.renderOrientation(this->getPosition(), this->getOrientation());
	if(renderBounds && this->getBounds()!=0)
	{
		glm::mat4 a = rManager.M;
		rManager.M = glm::translate(glm::mat4(), this->getPosition())*glm::toMat4(this->getOrientation());
		rManager.markMDirty();
		this->getBounds()->render(rManager, glm::vec4(1.f, 1.f, 0.f, 1.f), false);
		rManager.M = a;
	}
}
