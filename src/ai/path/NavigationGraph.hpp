#ifndef __PATHHOLDER_H_INCLUDED__
#define __PATHHOLDER_H_INCLUDED__

#include "PathCommon.h"

#include <vector>

#include "util/DeltaTime.hpp"
#include "render/RenderManager.hpp"

class NavigationGraph {
	public:
		std::vector<struct PathNode *> nodes;
		NavigationGraph();
		~NavigationGraph();
		void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
};

#endif
