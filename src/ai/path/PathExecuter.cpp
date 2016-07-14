#include "PathExecuter.hpp"

using namespace world;
using namespace util;

using namespace ai::path;

PathExecuter::PathExecuter(Entity *entity, NavigationGraph *navgraph)
{
	this->navgraph = navgraph;
	this->currentPathNode = -1;
	this->ent = entity;
	this->currentPath.clear();
	this->speed = 5.f;
}
PathExecuter::~PathExecuter()
{
	
}
void PathExecuter::postPath(const std::vector<int> &path)
{
	this->currentPath = path;
	this->distances = new float[this->currentPath.size()-1];
	this->timeToNextNode = new float[this->currentPath.size()-1];
	this->nodeTime = new float[this->currentPath.size()];
	this->nodeTime[0] = 0.f;
	for(unsigned long i=0;i<this->currentPath.size()-1;i++)
	{
		this->distances[i] = glm::distance(this->navgraph->nodes[this->currentPath[i]].position, this->navgraph->nodes[this->currentPath[i+1]].position);
		this->timeToNextNode[i] = this->distances[i]/this->speed;
		this->nodeTime[i+1] = this->nodeTime[i]+this->timeToNextNode[i];
	}
}
void PathExecuter::tick(DeltaTime &deltaTime)
{
	if(this->currentPath.size()==0)
		return;

	if(this->currentPathNode==-1)
	{
		this->currentPathNode = 0;
		this->pathStartTime = deltaTime.getTime();
	}
	double timeIntoPath;
	timeIntoPath = deltaTime.getTime()-this->pathStartTime;
	double nextNodeTime;
	nextNodeTime = this->nodeTime[this->currentPathNode+1];
	while(timeIntoPath>nextNodeTime)//If we've reached or gone past the current node
	{
		this->currentPathNode++;
		if((unsigned long)this->currentPathNode+1>=this->currentPath.size())
		{
			//this->currentPathNode = 0;
			//this->pathStartTime = deltaTime.getTime();
			//timeIntoPath = deltaTime.getTime()-this->pathStartTime;
			this->currentPath.clear();
			this->currentPathNode = -1;
			return;
		}
		nextNodeTime = this->nodeTime[this->currentPathNode+1];
	}
	
	double currentNodeTime = this->nodeTime[this->currentPathNode];
	double nextNodeTimeDelta = this->timeToNextNode[this->currentPathNode];
	double currentTimeBetweenNodes = timeIntoPath-currentNodeTime;
	glm::vec3 a = this->navgraph->nodes[this->currentPath[this->currentPathNode]].position;
	glm::vec3 b = this->navgraph->nodes[this->currentPath[this->currentPathNode+1]].position;
	this->ent->setPosition(a+(b-a)*(float)(currentTimeBetweenNodes/nextNodeTimeDelta));
	//this->currentPathNode++;
}
