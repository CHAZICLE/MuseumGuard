#include "PathExplorer.hpp"

using namespace ai::path;

PathExplorer::PathExplorer(NavigationGraph *navigationGraph)
{
	this->groupWhitelist = 0;
	this->poiGroupMask = 0;
	this->portalGroupMask = 0;
	this->lastPortalNode = 0;
	this->lastLastPortalNode = 0;
	this->navGraph = navigationGraph;
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
	this->poiGroupMask = groupMask;
}
void PathExplorer::setGroupPortal(int groupMask)
{
	this->portalGroupMask = groupMask;
}
//float PathExplorer::getNodeLinkWeight(PathNode *node, PathNodeLink *pathNodeLink, int depth)
//{
//	if(depth>4)
//		return 0;
//	float weight = 0;
//	if(this->visited.find(pathNodeLink->id)==this->visited.end())//If not visited
//		weight -= 1000/(1+depth);
//	PathNode *other = pathNodeLink->getOther(node);
//	float minWeight = std::numeric_limits<float>::max(),t;
//	PathNode *p;
//	for(auto *l : other->links)
//	{
//		p = l->getOther(other);
//		bool b = depth<2 && this->navGraph->getPathNodeGroupMask(p->id)&this->groupPOI && this->visitedPOI.find(p->id)==this->visitedPOI.end();
//		if(this->groupWhitelist==0 || this->navGraph->getPathNodeGroupMask(p->id)&this->groupWhitelist || b)
//		{
//			t = this->getNodeLinkWeight(node, l, depth+1);
//			if(b)
//				t -= 10000;
//			if(t<minWeight)
//			{
//				minWeight = t;
//			}
//		}
//	}
//	if(minWeight>=std::numeric_limits<float>::max()-1)
//		minWeight = 0;
//	return weight+minWeight;
//}
////Prefer: Unexplored, Shortest, Unexploded Children
//PathNode *PathExplorer::getNext_legacy(PathNode *current)
//{
//	PathNodeLink *selectedLink = 0,*backupLink = 0;
//	PathNode *selectedLinkNode = 0,*backupLinkNode=0,*p;
//	int count = 0;
//	float min = std::numeric_limits<float>::max(),t;
//	for(auto *l : current->links)
//	{
//		p = l->getOther(current);
//		bool b = this->navGraph->getPathNodeGroupMask(p->id)&this->groupPOI && this->visitedPOI.find(p->id)==this->visitedPOI.end();
//		if(this->groupWhitelist==0 || this->navGraph->getPathNodeGroupMask(p->id)&this->groupWhitelist || b)
//		{
//			if(l==this->previousVisitedLink)
//			{
//				backupLinkNode = p;
//				backupLink = l;
//				continue;
//			}
//			count++;
//			t = getNodeLinkWeight(current, l, 0);
//			if(b)
//				t -= 10000;
//			if(t<min)//If not visited
//			{
//				selectedLink = l;
//				selectedLinkNode = l->getOther(current);
//				min = t;
//			}
//		}
//		
//	}
//	if(count==0)
//	{
//		selectedLink = backupLink;
//		selectedLinkNode = backupLinkNode;
//	}
//	if(this->navGraph->getPathNodeGroupMask(selectedLinkNode->id)&this->groupPOI)
//	{
//		this->visitedPOI.insert(selectedLinkNode->id);
//	}
//	this->previousVisitedLink = selectedLink;
//	this->visited.insert(selectedLink->id);
//	return selectedLinkNode;
//}
inline std::string head(int len)
{
	std::string s;
	for(int i=0;i<len;i++)
		s += "	";
	s = s+std::to_string(len)+" ";
	return s;
}
PathNode *PathExplorer::getNext(PathNode *current)
{
	PathNode *selectedPortalNode=0,*other,*otherPortalNode=0;
	int portalNodeDepth;
	float min = std::numeric_limits<float>::max(),t;
	for(auto *l : current->links)
	{
		other = l->getOther(current);
		if(other==this->lastPortalNode || other==this->lastLastPortalNode)
			continue;
		otherPortalNode = 0;
		portalNodeDepth = 0;
		t = this->getNodeLinkWeight(l, other, &otherPortalNode, 1, portalNodeDepth, {}, 0)+std::rand()%5;
		if(min>t && otherPortalNode!=0)
		{
			min = t;
			selectedPortalNode = otherPortalNode;
		}
	}
	if(selectedPortalNode==0)
	{
		selectedPortalNode = this->lastPortalNode;
		if(selectedPortalNode==0)
			return 0;
	}
	else
	{
		this->previousVisitedPortalNodes.insert(selectedPortalNode->id);
	}
	this->lastLastPortalNode = this->lastPortalNode;
	this->lastPortalNode = selectedPortalNode;
	if(this->navGraph->getPathNodeGroupMask(selectedPortalNode->id)&this->poiGroupMask)
	{
		this->previousVisitedPOINodes.insert(selectedPortalNode->id);
	}
	return selectedPortalNode;
}
float PathExplorer::getNodeLinkWeight(PathNodeLink *pathNodeLink, PathNode *node, PathNode **selectedPortalNode, int depth, int &selectedPortalNodeDepth, std::set<int> previouslySearchedLinks, float weight)
{
	if(depth>5)
		return 0;
	weight += static_cast<float>(pathNodeLink->dist)/depth;
	previouslySearchedLinks.insert(pathNodeLink->id);
	if(depth<3 && this->navGraph->getPathNodeGroupMask(node->id)&this->poiGroupMask)
	{
		if(*selectedPortalNode==0)
		{
			*selectedPortalNode = node;
			selectedPortalNodeDepth = depth;
		}
		if(this->previousVisitedPOINodes.find(node->id)==this->previousVisitedPOINodes.end())//If we've already visited this POI node
			weight -= 10000;
		else
			weight += 100;
	}
	if(this->navGraph->getPathNodeGroupMask(node->id)&this->portalGroupMask)
	{
		if(*selectedPortalNode==0)
		{
			*selectedPortalNode = node;
			selectedPortalNodeDepth = depth;
		}
		if(this->previousVisitedPortalNodes.find(node->id)!=this->previousVisitedPortalNodes.end())//If we've already visited this portal node
			weight += 1000/depth;
	}
	if(node->links.size()==2)//Waypoint node
	{
		for(auto *l : node->links)
		{
			if(l!=pathNodeLink)//If this was NOT how we got here (its the other)
			{
				weight = this->getNodeLinkWeight(l, l->getOther(node), selectedPortalNode, depth+1, selectedPortalNodeDepth, previouslySearchedLinks, weight);
			}
		}
	}
	else// if(node->links.size()>2)//Junction node
	{
		int otherPortalNodeDepth;
		float min = std::numeric_limits<float>::max(),w;
		PathNode *otherPortalNode=0,*other,*selectedOtherPortalNode=0;
		for(auto *l : node->links)
		{
			if(l==pathNodeLink)//If this was how we got here
				continue;
			if(previouslySearchedLinks.find(l->id)!=previouslySearchedLinks.end())//If link was already searched
				continue;
			otherPortalNode = 0;
			other = l->getOther(node);
			w = this->getNodeLinkWeight(l, other, &otherPortalNode, depth+1, otherPortalNodeDepth, previouslySearchedLinks, 0);
			if(min>w)
			{
				min = w;
				selectedOtherPortalNode = otherPortalNode;
			}
		}
		if(*selectedPortalNode==0 && selectedOtherPortalNode!=0)
		{
			*selectedPortalNode = selectedOtherPortalNode;
		}
		if(selectedOtherPortalNode!=0)
			weight += min;
	}
	return weight;
}
