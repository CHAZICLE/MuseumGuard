#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "input/Controls.hpp"
#include "gui/screens/MainMenu.hpp"
#include "gui/ScreenManager.hpp"

#include "PathTest.hpp"

PathTest::PathTest() : Screen()
{
	this->pathHolder = new PathHolder();
	this->pathFinder = new PathFinder(this->pathHolder, this->pathHolder->nodes.at(0), this->pathHolder->nodes.at(50*50-1));
	this->lastTime = 0;
}
PathTest::~PathTest()
{
	
}
void PathTest::render(util::DeltaTime *deltaTime, render::RenderManager *manager)
{
	this->pathHolder->render(deltaTime, manager);
	/*if(this->lastTime<deltaTime->getTime()-0.1)
	{
		this->lastTime = deltaTime->getTime();
	}*/
	this->pathFinder->tick(1);
}
bool PathTest::onControlEvent(int control, int action)
{
	if((control&CONTROL_GUI_ESCAPE) && (action&CONTROL_KEYACTION_PRESS))
	{
		this->manager->openRootScreen(new MainMenu());
		return true;
	}
	return false;
}
