#include "gui/Screen.hpp"
#include <glm/glm.hpp>
#include <iostream>

#include "ScreenManager.hpp"

ScreenManager::ScreenManager()
{
	this->width = 0;
	this->height = 0;
}
ScreenManager::~ScreenManager()
{
	
}
void ScreenManager::openRootScreen(Screen *screen)
{
	while(!this->screens.empty())
	{
		Screen *screen = this->screens.front();
		delete screen;
		this->screens.pop_front();
	}
	screen->manager = this;
	this->screens.push_back(screen);
}
void ScreenManager::openScreen(Screen *screen)
{
	screen->manager = this;
	this->screens.push_front(screen);
}
bool ScreenManager::isScreenSurface(Screen *screen)
{
	return this->screens.front()==screen;
}
bool ScreenManager::isScreenBase(Screen *screen)
{
	return this->screens.back()==screen;
}
Screen *ScreenManager::closeScreen(Screen *screen)
{
	if(this->screens.size()<=1)
		return 0;
	Screen *s = this->screens.front();
	if(screen!=0 && s!=screen)
		return 0;
	this->screens.pop_front();
	return s;
}
bool ScreenManager::onControlEvent(int control, int action)
{
	for(std::list<Screen *>::reverse_iterator it = this->screens.rbegin(); it!=this->screens.rend(); it++)
	{
		Screen *screen = *it;
		if(screen->onControlEvent(control, action))
		{
			return true;
		}
	}
	return false;
}
bool ScreenManager::onControlEvent(int control, double x, double y, double dx, double dy)
{
	for(std::list<Screen *>::reverse_iterator it = this->screens.rbegin(); it!=this->screens.rend(); it++)
	{
		Screen *screen = *it;
		if(screen->onControlEvent(control, x, y, dx, dy))
			return true;
	}
	return false;
}
void ScreenManager::onScreenResize()
{
	for(std::list<Screen *>::iterator it = this->screens.begin(); it!=this->screens.end(); ++it)
	{
		Screen *screen = *it;
		screen->onScreenResize();
	}
}
void ScreenManager::render(double time, double fps, glm::mat4 matrix)
{
	for(std::list<Screen *>::iterator it = this->screens.begin(); it!=this->screens.end(); ++it)
	{
		Screen *screen = *it;
		screen->render(time, fps, matrix);
	}
}
double ScreenManager::getWidth()
{
	return this->width;
}
double ScreenManager::getHeight()
{
	return this->height;
}
