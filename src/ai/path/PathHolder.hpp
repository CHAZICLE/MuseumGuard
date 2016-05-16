#ifndef __PATHHOLDER_H_INCLUDED__
#define __PATHHOLDER_H_INCLUDED__

#include "PathCommon.h"

#include <vector>

namespace util {
class DeltaTime;
}
namespace render {
class RenderManager;
}

class PathHolder {
	public:
		std::vector<struct PathNode *> nodes;
		PathHolder();
		~PathHolder();
		void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
};

#endif
