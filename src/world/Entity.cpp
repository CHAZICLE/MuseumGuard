#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"

#include "Entity.hpp"

using namespace world;

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
void Entity::tick(util::DeltaTime &deltaTime)
{
	
}
void Entity::render(render::RenderManager &rManager)
{
	
}
void Entity::renderDebug(render::RenderManager &rManager, bool renderPosition, bool renderOrientation)
{
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	render::shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);

	// Draw the player orientation
	glm::vec3 playerDirection = this->getOrientation()*glm::vec3(0, 1, 0);
	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);
	render::BasicShapes::drawLine(this->getPosition(), this->getPosition()+playerDirection, vploc);

	glUniform4f(loc, 0.0f, 0.0f, 1.0f, 1.0f);
	render::BasicShapes::drawLine(this->getPosition()+playerDirection, this->getPosition()+playerDirection+this->getOrientation()*glm::vec3(0, 0, 0.2), vploc);
}
