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
void Entity::render(render::RenderManager *manager)
{
	
}
