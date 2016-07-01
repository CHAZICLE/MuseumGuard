#ifndef __DEBUGPATHHOLDER_H_INCLUDED__
#define __DEBUGPATHHOLDER_H_INCLUDED__

#include "PathCommon.h"

#include <vector>

#include "util/DeltaTime.hpp"
#include "render/RenderManager.hpp"

namespace ai {
	namespace path {
		class DebugPathHolder {
			public:
				std::vector<struct PathNode *> nodes;
				DebugPathHolder();
				~DebugPathHolder();
				void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		};
	}
}


#endif
