#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"

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
