#include "PathExplorer.hpp"

using namespace ai::path;

PathExplorer::PathExplorer(NavigationGraph *navigationGraph)
{
	this->navGraph = navigationGraph;
	this->groupWhitelist = 0;
}
PathExplorer::~PathExplorer()
{
	
}
void PathExplorer::setGroupWhitelist(int groupMask)
{
	this->groupWhitelist = groupMask;
}
void PathExplorer::setGroupPOI(int groupMask)
{
	this->groupPOI = groupMask;
}
float PathExplorer::getNodeLinkWeight(PathNode *node, PathNodeLink *pathNodeLink, int depth)
{
	if(depth>4)
		return 0;
	float weight = 0;
	if(this->visited.find(pathNodeLink->id)==this->visited.end())//If not visited
		weight -= 1000/(1+depth);
	PathNode *other = pathNodeLink->getOther(node);
	float minWeight = std::numeric_limits<float>::max(),t;
	PathNode *p;
	for(auto *l : other->links)
	{
		p = l->getOther(other);
		bool b = depth<2 && this->navGraph->getPathNodeGroupMask(p->id)&this->groupPOI && this->visitedPOI.find(p->id)==this->visitedPOI.end();
		if(this->groupWhitelist==0 || this->navGraph->getPathNodeGroupMask(p->id)&this->groupWhitelist || b)
		{
			t = this->getNodeLinkWeight(node, l, depth+1);
			if(b)
				t -= 10000;
			if(t<minWeight)
			{
				minWeight = t;
			}
		}
	}
	if(minWeight>=std::numeric_limits<float>::max()-1)
		minWeight = 0;
	return weight+minWeight;
}
//Prefer: Unexplored, Shortest, Unexploded Children
PathNode *PathExplorer::getNext(PathNode *current)
{
	PathNodeLink *selectedLink = 0,*backupLink = 0;
	PathNode *selectedLinkNode = 0,*backupLinkNode=0,*p;
	int count = 0;
	float min = std::numeric_limits<float>::max(),t;
	for(auto *l : current->links)
	{
		p = l->getOther(current);
		bool b = this->navGraph->getPathNodeGroupMask(p->id)&this->groupPOI && this->visitedPOI.find(p->id)==this->visitedPOI.end();
		if(this->groupWhitelist==0 || this->navGraph->getPathNodeGroupMask(p->id)&this->groupWhitelist || b)
		{
			if(l==this->previousVisitedLink)
			{
				backupLinkNode = p;
				backupLink = l;
				continue;
			}
			count++;
			t = getNodeLinkWeight(current, l, 0);
			if(b)
				t -= 10000;
			if(t<min)//If not visited
			{
				selectedLink = l;
				selectedLinkNode = l->getOther(current);
				min = t;
			}
		}
		
	}
	if(count==0)
	{
		selectedLink = backupLink;
		selectedLinkNode = backupLinkNode;
	}
	if(this->navGraph->getPathNodeGroupMask(selectedLinkNode->id)&this->groupPOI)
	{
		this->visitedPOI.insert(selectedLinkNode->id);
	}
	this->previousVisitedLink = selectedLink;
	this->visited.insert(selectedLink->id);
	return selectedLinkNode;
}
