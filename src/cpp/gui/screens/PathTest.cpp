#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "input/Controls.hpp"
#include "gui/screens/MainMenu.hpp"
#include "gui/ScreenManager.hpp"
#include "ai/path/DebugPathHolder.hpp"

#include "PathTest.hpp"

using namespace ai::path;

PathTest::PathTest() : Screen()
{
	this->pathHolder = new DebugPathHolder();
	this->pathFinder = new PathFinder();
	this->pathFinder->start(this->pathHolder->nodes.at(0), this->pathHolder->nodes.at(50*50-1));
	this->lastTime = 0;
}
PathTest::~PathTest()
{
	delete this->pathHolder;
	delete this->pathFinder;
}
void PathTest::render(util::DeltaTime &deltaTime, render::RenderManager &manager)
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
