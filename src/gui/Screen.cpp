#include <glm/glm.hpp>
#include "Element.hpp"

#include "Screen.hpp"

Screen::Screen()
{
	this->selectedElement = 0;
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
bool Screen::onControlEvent(int control, bool state)
{
	if(state)
	{
		if(this->selectedElement!=0)
		{
			if(control&CONTROL_GUI_NEXT && this->selectedElement->next!=0)
			{
				this->selectedElement = this->selectedElement->next;
				return true;
			}
			if(control&CONTROL_GUI_PREV && this->selectedElement->prev!=0)
			{
				this->selectedElement = this->selectedElement->prev;
				return true;
			}
			if(control&CONTROL_GUI_LEFT && this->selectedElement->left!=0)
			{
				this->selectedElement = this->selectedElement->left;
				return true;
			}
			if(control&CONTROL_GUI_RIGHT && this->selectedElement->right!=0)
			{
				this->selectedElement = this->selectedElement->right;
				return true;
			}
			if(control&CONTROL_GUI_UP && this->selectedElement->up!=0)
			{
				this->selectedElement = this->selectedElement->up;
				return true;
			}
			if(control&CONTROL_GUI_DOWN && this->selectedElement->down!=0)
			{
				this->selectedElement = this->selectedElement->down;
				return true;
			}
		}
		else
		{
			if(control&CONTROL_GUI_NEXT && this->firstNext!=0)
			{
				this->selectedElement = this->firstNext;
				return true;
			}
			if(control&CONTROL_GUI_PREV && this->firstPrev!=0)
			{
				this->selectedElement = this->firstPrev;
				return true;
			}
			if(control&CONTROL_GUI_LEFT && this->firstLeft!=0)
			{
				this->selectedElement = this->firstLeft;
				return true;
			}
			if(control&CONTROL_GUI_RIGHT && this->firstRight!=0)
			{
				this->selectedElement = this->firstRight;
				return true;
			}
			if(control&CONTROL_GUI_UP && this->firstUp!=0)
			{
				this->selectedElement = this->firstUp;
				return true;
			}
			if(control&CONTROL_GUI_DOWN && this->firstDown!=0)
			{
				this->selectedElement = this->firstDown;
				return true;
			}
		}
	}
	return this->selectedElement->onControlEvent(control, state);
}
bool Screen::onControlEvent(int control, double x, double y, double dx, double dy)
{
	for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
	{
		Element *e = *it;
		if(e->onControlEvent(control, x, y, dx, dy))
			return true;
	}
	return false;
}
void Screen::onResize()
{
	for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
	{
		Element *e = *it;
		e->onResize();
	}
}
void Screen::render(double time, double fps, glm::mat4 matrix)
{
	for(std::list<Element *>::iterator it = this->elements.begin(); it!=this->elements.end(); ++it)
	{
		Element *e = *it;
		e->render(time, fps, matrix);
	}
}
