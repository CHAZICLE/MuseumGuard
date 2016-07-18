#ifndef FYP_AI_PATH_PATHEXPLORER_HPP_
#define FYP_AI_PATH_PATHEXPLORER_HPP_

#include <set>
#include "PathCommon.h"
#include "ai/path/NavigationGraph.hpp"

namespace ai {
	namespace path {
		class PathExplorer {
			private:
				bool exploreEdges;
				int groupWhitelist,groupPOI;
				PathNode *startNode;
				PathNodeLink *previousVisitedLink;
				std::set<int> visited;
				std::set<int> visitedPOI;
				NavigationGraph *navGraph;
			public:
				PathExplorer(NavigationGraph *navigationGraph);
				~PathExplorer();
				void setGroupWhitelist(int groupMask);
				void setGroupPOI(int groupMask);
				PathNode *getNext(PathNode *current);
				float getNodeLinkWeight(PathNode *node, PathNodeLink *pathNodeLink, int depth);
		};
	}
}

#endif
