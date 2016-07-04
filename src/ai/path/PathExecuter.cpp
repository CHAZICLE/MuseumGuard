#include "PathExecuter.hpp"

using namespace world;
using namespace util;

using namespace ai::path;

PathExecuter::PathExecuter(Entity *entity)
{
	this->ent = entity;
	this->currentPath.clear();
}
PathExecuter::~PathExecuter()
{
	
}
void PathExecuter::postPath(const std::vector<int> &path)
{
	this->currentPath = path;
}
void PathExecuter::tick(DeltaTime &deltaTime)
{
}
