#ifndef FYP_AI_PATH_PATHEXPLORER_HPP_
#define FYP_AI_PATH_PATHEXPLORER_HPP_

#include <set>
#include "PathCommon.h"
#include "ai/path/NavigationGraph.hpp"

namespace ai {
	namespace path {
		class PathExplorer {
			private:
				int groupWhitelist,poiGroupMask,portalGroupMask;
				PathNode *lastPortalNode,*lastLastPortalNode;
				std::set<int> previousVisitedPortalNodes;
				std::set<int> previousVisitedPOINodes;
				NavigationGraph *navGraph;
			public:
				PathExplorer(NavigationGraph *navigationGraph);
				~PathExplorer();
				void setGroupWhitelist(int groupMask);
				void setGroupPOI(int groupMask);
				void setGroupPortal(int groupMask);
				PathNode *getNext_legacy(PathNode *current);
				float getNodeLinkWeight_legacy(PathNode *node, PathNodeLink *pathNodeLink, int depth);
				PathNode *getNext(PathNode *current);
				float getNodeLinkWeight(PathNodeLink *pathNodeLink, PathNode *node, PathNode **selectedPortalNode, int depth, int &portalNodeDepth, std::set<int> previouslySearchedLinks, float weight);
		};
	}
}

#endif
