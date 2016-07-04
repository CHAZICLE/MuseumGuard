#ifndef __PATHEXECUTER_H_INCLUDED__
#define __PATHEXECUTER_H_INCLUDED__

#include <vector>
#include "world/Entity.hpp"
#include "util/DeltaTime.hpp"

namespace ai {
	namespace path {
		class PathExecuter {
			private:
				std::vector<int> currentPath;
				world::Entity *ent;
			public:
				PathExecuter(world::Entity *entity);
				~PathExecuter();
				void postPath(const std::vector<int> &path);
				void tick(util::DeltaTime &deltaTime);
		};
	}
}

#endif
