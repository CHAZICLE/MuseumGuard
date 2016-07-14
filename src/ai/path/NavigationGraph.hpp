#ifndef FYP_AI_PATH_NAVIGATIONGRAPH_HPP_
#define FYP_AI_PATH_NAVIGATIONGRAPH_HPP_

#include "PathCommon.h"

#include <vector>

#include "util/AssetManager.hpp"

#include "render/RenderManager.hpp"


namespace ai {
	namespace path {
		class NavigationGraph : public util::Asset {
			public:
				int numGroups;
				int numNodes;
				int *groupCounts;
				int *groupMasks;
				PathNode *nodes;
				NavigationGraph(int assetId, std::istream &fp);
				virtual ~NavigationGraph();
				virtual void write(std::ostream &ost) const;
				virtual void postload();
				void render(render::RenderManager &manager);
				PathNode *getRandomNode(int mask);
				PathNode *getNearestPathNode(glm::vec3 position, int mask);
				PathNode *getNearestPathNode(glm::vec3 position);
			private:
				// Debug
				GLuint vaid,vpbid,vcbid;
				GLfloat *vpd;
				GLfloat *vcd;
		};
}
}

#endif
