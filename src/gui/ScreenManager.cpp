#include "gui/Screen.hpp"
#include <glm/glm.hpp>

#include "ScreenManager.hpp"

ScreenManager::ScreenManager()
{
	
}
ScreenManager::~ScreenManager()
{
	
}
void ScreenManager::openRootScreen(Screen *screen)
{
	while(!this->screens.empty())
	{
		delete this->screens.front();
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
bool ScreenManager::onControlEvent(int control, bool state)
{
	for(std::list<Screen *>::iterator it = this->screens.end(); it!=this->screens.begin(); ++it)
	{
		Screen *screen = *it;
		if(screen->onControlEvent(control, state))
			return true;
	}
	return false;
}
bool ScreenManager::onControlEvent(int control, double x, double y, double dx, double dy)
{
	for(std::list<Screen *>::iterator it = this->screens.end(); it!=this->screens.begin(); ++it)
	{
		Screen *screen = *it;
		if(screen->onControlEvent(control, x, y, dx, dy))
			return true;
	}
	return false;
}
void ScreenManager::onResize()
{
	for(std::list<Screen *>::iterator it = this->screens.begin(); it!=this->screens.end(); ++it)
	{
		Screen *screen = *it;
		screen->onResize();
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
