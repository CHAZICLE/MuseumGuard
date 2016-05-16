#include "gui/Screen.hpp"
#include <glm/glm.hpp>
#include "input/Controls.hpp"

#include "ScreenManager.hpp"

ScreenManager::ScreenManager()
{
	this->width = 0;
	this->height = 0;
	this->lastCursorX = -1;
	this->lastCursorY = -1;
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
	this->onSurfaceScreenChanged(screen);
}
void ScreenManager::openScreen(Screen *screen)
{
	screen->manager = this;
	this->screens.push_back(screen);
	this->onSurfaceScreenChanged(screen);
}
void ScreenManager::close()
{

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
	Screen *s = this->screens.back();
	if(screen!=0 && s!=screen)
		return 0;
	this->screens.pop_back();
	this->onSurfaceScreenChanged(this->screens.front());
	return s;
}
#include <iostream>
#include <typeinfo>
bool ScreenManager::onControlEvent(int control, int action)
{
	//std::cout << "EVENT: control=" << control << ", action=" << action << ", mask=" << (action&CONTROL_KEYACTION_RELEASE) << std::endl;
	for(std::list<Screen *>::reverse_iterator it = this->screens.rbegin(); it!=this->screens.rend(); ++it)
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
	this->lastCursorX = x;
	this->lastCursorY = y;
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
void ScreenManager::onSurfaceScreenChanged(Screen *screen)
{
	screen->onControlEvent(CONTROL_ACTION_MOUSE, this->lastCursorX, this->lastCursorY, 0, 0);
}
void ScreenManager::render(util::DeltaTime &deltaTime, render::RenderManager &manager)
{
	for(std::list<Screen *>::iterator it = this->screens.begin(); it!=this->screens.end(); ++it)
	{
		Screen *screen = *it;
		screen->render(deltaTime, manager);
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
