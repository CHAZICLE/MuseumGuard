#ifndef __PATHTEST_H_INCLUDED__
#define __PATHTEST_H_INCLUDED__

#include "ai/path/DebugPathHolder.hpp"
#include "ai/path/PathFinder.hpp"
#include "gui/Screen.hpp"
namespace util {
class DeltaTime;
}
namespace render {
class RenderManager;
}

class PathTest : public Screen {
	public:
		PathTest();
		~PathTest();
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		ai::path::DebugPathHolder *pathHolder;
		PathFinder *pathFinder;
		double lastTime;
		virtual bool onControlEvent(int control, int action);
};

#endif
