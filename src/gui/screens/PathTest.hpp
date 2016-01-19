#ifndef __PATHTEST_H_INCLUDED__
#define __PATHTEST_H_INCLUDED__

#include "ai/path/PathHolder.hpp"
#include "ai/path/PathFinder.hpp"
#include "gui/Screen.hpp"

class PathTest : public Screen {
	public:
		PathTest();
		~PathTest();
		virtual void render(double time, double fps, glm::mat4 matrix);
		PathHolder *pathHolder;
		PathFinder *pathFinder;
		double lastTime;
};

#endif
