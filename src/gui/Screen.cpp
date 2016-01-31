#include <glm/glm.hpp>
#include "Element.hpp"
#include "input/Controls.hpp"
#include <iostream>

#include "Screen.hpp"

Screen::Screen()
{
	this->selectedElement = 0;
	this->elementSelectedWithMouse = false;
	this->firstNext = 0;
	this->firstPrev = 0;
	this->firstLeft = 0;
	this->firstRight = 0;
	this->firstUp = 0;
	this->firstDown = 0;
	this->manager = 0;
}
Screen::~Screen()
{
	
}
void Screen::addElement(Element *e)
{
	this->elements.push_back(e);
	e->screen = this;
}
void Screen::render(util::DeltaTime *deltaTime, render::RenderManager *manager)
{
	for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
	{
		Element *e = *it;
		e->render(deltaTime, manager);
	}
}
void Screen::selectElement(Element *element, bool mouseSelection)
{
	if(this->selectedElement!=0)
	{
		this->selectedElement->selected = false;
	}
	this->selectedElement = element;
	if(this->selectedElement!=0)
	{
		this->selectedElement->selected = true;
	}
	this->elementSelectedWithMouse = mouseSelection;
}
bool Screen::onControlEvent(int control, int action)
{
	if(action==CONTROL_ACTION_PRESS)
	{
		if(this->selectedElement!=0)
		{
			if((control&CONTROL_GUI_NEXT) && this->selectedElement->next!=0)
			{
				this->selectElement(this->selectedElement->next, false);
				return true;
			}
			if((control&CONTROL_GUI_PREV) && this->selectedElement->prev!=0)
			{
				this->selectElement(this->selectedElement->prev, false);
				return true;
			}
			if((control&CONTROL_GUI_LEFT) && this->selectedElement->left!=0)
			{
				this->selectElement(this->selectedElement->left, false);
				return true;
			}
			if((control&CONTROL_GUI_RIGHT) && this->selectedElement->right!=0)
			{
				this->selectElement(this->selectedElement->right, false);
				return true;
			}
			if((control&CONTROL_GUI_UP) && this->selectedElement->up!=0)
			{
				this->selectElement(this->selectedElement->up, false);
				return true;
			}
			if((control&CONTROL_GUI_DOWN) && this->selectedElement->down!=0)
			{
				this->selectElement(this->selectedElement->down, false);
				return true;
			}
		}
		else
		{
			if((control&CONTROL_GUI_NEXT) && this->firstNext!=0)
			{
				this->selectElement(this->firstNext, false);
				return true;
			}
			if((control&CONTROL_GUI_PREV) && this->firstPrev!=0)
			{
				this->selectElement(this->firstPrev, false);
				return true;
			}
			if((control&CONTROL_GUI_LEFT) && this->firstLeft!=0)
			{
				this->selectElement(this->firstLeft, false);
				return true;
			}
			if((control&CONTROL_GUI_RIGHT) && this->firstRight!=0)
			{
				this->selectElement(this->firstRight, false);
				return true;
			}
			if((control&CONTROL_GUI_UP) && this->firstUp!=0)
			{
				this->selectElement(this->firstUp, false);
				return true;
			}
			if((control&CONTROL_GUI_DOWN) && this->firstDown!=0)
			{
				this->selectElement(this->firstDown, false);
				return true;
			}
		}
	}
	if(this->selectedElement!=0)
	{
		return this->selectedElement->onControlEvent(control, action);
	}
	return false;
}
bool Screen::onControlEvent(int control, double x, double y, double dx, double dy)
{
	if(control==CONTROL_ACTION_MOUSE)
	{
		if(this->selectedElement!=0 && this->elementSelectedWithMouse)
		{
			// Mouse de-selection only if selected with mouse
			if(!this->selectedElement->isInside(x, y))
				this->selectElement(0, true);
		}
		for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
		{
			Element *e = *it;
			if(e->isInside(x, y))
			{
				this->selectElement(e, true);
			}
		}
	}
	if(this->selectedElement!=0)
	{
		return this->selectedElement->onControlEvent(control, x, y, dx, dy);
	}
	return false;
}
void Screen::onScreenResize()
{
	for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
	{
		Element *e = *it;
		e->onScreenResize();
	}
}

bool Screen::supportsCursor() {
	return true;
}
