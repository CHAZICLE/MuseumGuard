#ifndef FYP_AI_PATH_PATHFINDER_HPP_
#define FYP_AI_PATH_PATHFINDER_HPP_

#include "PathCommon.h"
#include <set>
#include <map>
#include <mutex>
#include <unordered_set>

namespace ai {
	namespace path {
		struct StoredPathNode {
			struct PathNodeLink *parentLink;
			struct StoredPathNode *parent;
			struct PathNode *node;
			float f,g,h;
		};
		class PathFinder {
			private:
				StoredPathNode *startPathNode,*endPathNode,*c;
			public:
				std::unordered_set<glm::vec3> dangerPoints;
				float dangerRange,dangerCost;
				int iterations;
				std::map<int, StoredPathNode *> storedPathNodes;
				std::set<StoredPathNode *> openSet;
				std::set<StoredPathNode *> closedSet;
				float calculateDangerOffset(PathNode *pathNode);
				PathFinder();
				~PathFinder();
				void start(PathNode *a, PathNode *b);
				bool tick(int i);
				std::vector<int> getPath();
				StoredPathNode *getStoredNode(PathNode *node);
				PathNode *getCurrentNode();
				bool done,sucess;
		};
	}
}

#endif
