#include "PathFinder.hpp"

PathFinder::PathFinder(PathHolder *holder, struct PathNode *start, struct PathNode *end)
{
	this->iterations = 0;
	// Remember the algorithm works backwards
	this->start = this->getStoredNode(end);
	this->end = this->getStoredNode(start);

	this->start->f = glm::distance(this->start->node->position, this->end->node->position);
	this->end->f = 0;

	this->openSet.insert(this->start);
	start->current = true;
	end->current = true;
	this->done = false;
}
PathFinder::~PathFinder()
{
	
}
bool PathFinder::tick(int ticks)
{
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
		int min = std::numeric_limits<int>::max();
		for(std::set<struct StoredPathNode *>::iterator openSetIterator = this->openSet.begin(); openSetIterator != this->openSet.end(); openSetIterator++)
		{
			struct StoredPathNode *s_node = *openSetIterator;
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
		return true;
	}
	if(c==this->end)
	{
		// Done: Complete path
		this->done = true;
		return true;
	}
	this->openSet.erase(c);
	this->closedSet.insert(c);
	c->node->open = false;
	c->node->closed = true;
	float newNeighbourG;
	// Add its parents to the open set
	for(std::vector<struct PathNodeLink *>::iterator pnLinkIt = c->node->links.begin(); pnLinkIt!=c->node->links.end(); ++pnLinkIt)
	{
		struct PathNodeLink *pnLink = *pnLinkIt;
		// Get neighbour
		struct PathNode *otherNode = pnLink->a;
		if(otherNode==c->node)
			otherNode = pnLink->b;
		// Get neighbour node
		struct StoredPathNode *neighbourNode = getStoredNode(otherNode);
		// Skip neighbours in closed set
		if(this->closedSet.find(neighbourNode)!=this->closedSet.end())
			continue;
		newNeighbourG = c->g+pnLink->dist/10;
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
		neighbourNode->h = glm::distance(neighbourNode->node->position, this->end->node->position);
		neighbourNode->f = neighbourNode->g+neighbourNode->h;
	}
	return false;
}
struct StoredPathNode *PathFinder::getStoredNode(struct PathNode *node)
{
	struct StoredPathNode *s_node = this->storedPathNodes[node->id];
	if(s_node==0)
	{
		s_node = new struct StoredPathNode;
		s_node->parent = 0;
		s_node->node = node;
		s_node->f = 0;
		s_node->g = 0;
		s_node->h = 0;
		this->storedPathNodes[node->id] = s_node;
	}
	return s_node;
}
struct PathNode *PathFinder::getCurrentNode()
{
	if(this->c==0)
		return 0;
	return this->c->node;
}
