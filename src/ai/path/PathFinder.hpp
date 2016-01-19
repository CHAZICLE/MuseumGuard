#ifndef __PATHFINDER_H_INCLUDED__
#define __PATHFINDER_H_INCLUDED__

#include "PathCommon.h"
#include "PathHolder.hpp"
#include <set>
#include <map>

struct StoredPathNode {
	struct PathNodeLink *parentLink;
	struct StoredPathNode *parent;
	struct PathNode *node;
	float f,g,h;
};

class PathFinder {
	private:
		struct StoredPathNode *start,*end,*c;
	public:
		int iterations;
		std::map<int, struct StoredPathNode *> storedPathNodes;
		std::set<struct StoredPathNode *> openSet;
		std::set<struct StoredPathNode *> closedSet;
		PathFinder(PathHolder *holder, struct PathNode *start, struct PathNode *end);
		~PathFinder();
		bool tick(int i);
		struct StoredPathNode *getStoredNode(struct PathNode *node);
		struct PathNode *getCurrentNode();
		bool done;

};

#endif
