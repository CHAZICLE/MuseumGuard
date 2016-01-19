#ifndef __PATHHOLDER_H_INCLUDED__
#define __PATHHOLDER_H_INCLUDED__

#include "PathCommon.h"

#include <vector>

class PathHolder {
	public:
		std::vector<struct PathNode *> nodes;
		PathHolder();
		~PathHolder();
		void render(glm::mat4 matrix);
};

#endif
