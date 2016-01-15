#include "Element.hpp"

Element::Element()
{
	this->screen = 0;
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
}
void Element::setY(float y)
{
	this->y = y;
}
void Element::setWidth(float width)
{
	this->width = width;
}
void Element::setHeight(float height)
{
	this->height = height;
}
void Element::setSize(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
