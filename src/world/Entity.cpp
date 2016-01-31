#include "Entity.hpp"

Entity::Entity()
{
	
}
Entity::~Entity()
{
	
}
glm::vec3 Entity::getPosition()
{
	return this->position;
}
glm::quat Entity::getRotation()
{
	return this->rotation;
}
void Entity::teleportTo(glm::vec3 location)
{
	this->position = location;
}
void Entity::rotateTo(glm::quat rotation)
{
	this->rotation = rotation;
}
void Entity::render(render::RenderManager *manager)
{
	
}
