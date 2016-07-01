#ifndef __PATHFINDER_H_INCLUDED__
#define __PATHFINDER_H_INCLUDED__

#include "PathCommon.h"
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
		StoredPathNode *start,*end,*c;
	public:
		int iterations;
		std::map<int, StoredPathNode *> storedPathNodes;
		std::set<StoredPathNode *> openSet;
		std::set<StoredPathNode *> closedSet;
		PathFinder(PathNode *start, PathNode *end);
		~PathFinder();
		bool tick(int i);
		StoredPathNode *getStoredNode(PathNode *node);
		PathNode *getCurrentNode();
		bool done;

};

#endif
