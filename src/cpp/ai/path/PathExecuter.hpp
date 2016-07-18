#ifndef FYP_AI_PATH_PATHEXECUTER_HPP_
#define FYP_AI_PATH_PATHEXECUTER_HPP_

#include <vector>
#include "world/Entity.hpp"
#include "util/DeltaTime.hpp"
#include "ai/path/NavigationGraph.hpp"

namespace ai {
	namespace path {
		class PathExecuter {
			private:
				std::vector<int> currentPath;
				float *distances;
				float *timeToNextNode;
				float *nodeTime;
				world::Entity *ent;
				long currentPathNode;
				NavigationGraph *navgraph;
				float speed;
				double pathStartTime;
			public:
				PathExecuter(float movementSpeed, world::Entity *entity, NavigationGraph *navgraph);
				~PathExecuter();
				void postPath(const std::vector<int> &path);
				void tick(util::DeltaTime &deltaTime);
		};
	}
}

#endif
