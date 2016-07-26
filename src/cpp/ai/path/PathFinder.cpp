#include "PathFinder.hpp"

using namespace ai::path;

PathFinder::PathFinder()
{
	this->startPathNode = 0;
	this->storedPathNodes.clear();
	this->done = false;
	this->sucess = false;
}
PathFinder::~PathFinder()
{
	for(auto &a : this->storedPathNodes)
		delete a.second;
}
void PathFinder::start(PathNode *a, PathNode *b)
{
	this->iterations = 0;
	// Remember the algorithm works backwards
	PRINT_DEBUG("a");
	this->startPathNode = this->getStoredNode(b);
	this->endPathNode = this->getStoredNode(a);
	PRINT_DEBUG("b");

	this->startPathNode->f = glm::distance(this->startPathNode->node->position, this->endPathNode->node->position);
	this->endPathNode->f = 0;

	this->openSet.clear();
	this->closedSet.clear();
	this->openSet.insert(this->startPathNode);
	a->current = true;
	b->current = true;
	this->done = false;
	this->sucess = false;
}
bool PathFinder::tick(int ticks)
{
	if(this->startPathNode==0)
		return false;
	if(this->done)
		return true;
	if(ticks!=1)
	{
		for(int i=1;i<ticks;i++)
		{
			if(this->tick(1))
				return true;
		}
	}
	this->iterations++;
	this->c = 0;
	// Find a node in the open set with the lowest f cost
	if(!this->openSet.empty())
	{
		float min = std::numeric_limits<float>::max();
		for(std::set<StoredPathNode *>::iterator openSetIterator = this->openSet.begin(); openSetIterator != this->openSet.end(); openSetIterator++)
		{
			StoredPathNode *s_node = *openSetIterator;
			if(s_node->f<min)
			{
				min = s_node->f;
				c = s_node;
			}
		}
	}
	// Whats the new node we found in open set
	if(c==0)
	{
		// Done: Failed
		this->done = true;
		this->sucess = false;
		return true;
	}
	if(c==this->endPathNode)
	{
		// Done: Complete path
		this->done = true;
		this->sucess = true;
		c = this->endPathNode;
		while(c!=this->startPathNode)
		{
			c->node->current = true;
			c = c->parent;
		}
		return true;
	}
	this->openSet.erase(c);
	this->closedSet.insert(c);
	c->node->open = false;
	c->node->closed = true;
	float newNeighbourG;
	// Add its parents to the open set
	for(std::vector<PathNodeLink *>::iterator pnLinkIt = c->node->links.begin(); pnLinkIt!=c->node->links.end(); ++pnLinkIt)
	{
		PathNodeLink *pnLink = *pnLinkIt;
		// Get neighbour
		PathNode *otherNode = pnLink->a;
		if(otherNode==c->node)
			otherNode = pnLink->b;
		// Get neighbour node
		StoredPathNode *neighbourNode = getStoredNode(otherNode);
		// Skip neighbours in closed set
		if(this->closedSet.find(neighbourNode)!=this->closedSet.end())
			continue;
		newNeighbourG = c->g+static_cast<float>(pnLink->dist);
		// Add to open set if not already in
		if(this->openSet.find(neighbourNode)==this->openSet.end())
		{
			this->openSet.insert(neighbourNode);
			neighbourNode->node->open = true;
		}
		// Determine if this path is worse
		else
		{
			if(newNeighbourG >= neighbourNode->g)
				continue;//Skip worse path
		}
		// Set up neighbour costs
		neighbourNode->parent = c;
		neighbourNode->parentLink = pnLink;
		neighbourNode->g = newNeighbourG;
		neighbourNode->h = glm::distance(neighbourNode->node->position, this->endPathNode->node->position);
		neighbourNode->f = neighbourNode->g+neighbourNode->h;
	}
	return false;
}
std::vector<int> PathFinder::getPath()
{
	std::vector<int> path;
	this->c = this->endPathNode;
	path.push_back(this->c->node->id);
	while(this->c!=this->startPathNode)
	{
		this->c = this->c->parent;
		path.push_back(this->c->node->id);
	}
	return path;
}
StoredPathNode *PathFinder::getStoredNode(PathNode *node)
{
	if(this->storedPathNodes.find(node->id)==this->storedPathNodes.end())
	{
		StoredPathNode *s_node;
		s_node = new StoredPathNode;
		s_node->parent = 0;
		s_node->node = node;
		s_node->f = 0;
		s_node->g = 0;
		this->storedPathNodes[node->id] = s_node;
		return s_node;
	}
	return this->storedPathNodes[node->id];
}
PathNode *PathFinder::getCurrentNode()
{
	if(this->c==0)
		return 0;
	return this->c->node;
}
