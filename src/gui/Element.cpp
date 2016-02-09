#include "util/gl.h"
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/RenderManager.hpp"

#include "Element.hpp"

Element::Element()
{
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
	this->screen = 0;
	this->next = 0;
	this->prev = 0;
	this->left = 0;
	this->right = 0;
	this->up = 0;
	this->down = 0;
	this->selected = false;
}
Element::~Element()
{
	
}
float Element::getX()
{
	return this->x;
}
float Element::getY()
{
	return this->y;
}
float Element::getWidth()
{
	return this->width;
}
float Element::getHeight()
{
	return this->height;
}
Element *Element::setX(float x)
{
	this->x = x;
	this->onElementResize();
}
Element *Element::setY(float y)
{
	this->y = y;
	this->onElementResize();
}
Element *Element::setWidth(float width)
{
	this->width = width;
	this->onElementResize();
}
Element *Element::setHeight(float height)
{
	this->height = height;
	this->onElementResize();
}
Element *Element::setSize(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->onElementResize();
}
bool Element::isInside(float x, float y)
{
	return this->getX()<=x
		&& this->getX()+this->getWidth()>=x
		&& this->getY()<=y
		&& this->getY()+this->getHeight()>=y;
}
void Element::render(util::DeltaTime *deltaTime, render::RenderManager *manager)
{
	glUseProgram(shaders::program_solidcolor);
	manager->setMVPMatrix(shaders::program_solidcolor_MVP);
	
	BasicShapes::renderUnitSquare(shaders::program_solidcolor_vertexPosition);
}
void Element::onElementResize()
{
}
bool Element::onControlEvent(int control, int action)
{
	return false;
}
bool Element::onControlEvent(int control, double x, double y, double dx, double dy)
{
	return false;
}
void Element::onScreenResize()
{
}
