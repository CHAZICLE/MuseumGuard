#include "util/gl.h"
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <iostream>

#include "Element.hpp"

Element::Element()
{
	this->screen = 0;
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
void Element::setX(float x)
{
	this->x = x;
	this->onElementResize();
}
void Element::setY(float y)
{
	this->y = y;
	this->onElementResize();
}
void Element::setWidth(float width)
{
	this->width = width;
	this->onElementResize();
}
void Element::setHeight(float height)
{
	this->height = height;
	this->onElementResize();
}
void Element::setSize(float x, float y, float width, float height)
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
void Element::render(double time, double fps, glm::mat4 matrix)
{
	glUseProgram(shaders::program_solidcolor);
	glUniformMatrix4fv(shaders::program_solidcolor_MVP, 1, GL_FALSE, &matrix[0][0]);
	
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
