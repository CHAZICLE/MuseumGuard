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
void PathTest::render(double time, double fps, glm::mat4 matrix)
{
	this->pathHolder->render(matrix);
	if(this->lastTime<time-0.1)
	{
		this->lastTime = time;
		this->pathFinder->tick(1);
	}
}
