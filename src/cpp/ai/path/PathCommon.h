#ifndef FYP_AI_PATH_PATHCOMMON_H_
#define FYP_AI_PATH_PATHCOMMON_H_

#include <glm/glm.hpp>
#include <vector>
#include "util/DebugFlags.hpp"

#define PATHNODE_FLAG_ENTRANCE 1
#define PATHNODE_FLAG_EXIT 2
#define PATHNODE_FLAG_COVER 4
#define PATHNODE_FLAG_EXHIBIT 8

struct PathNode;

struct PathNodeLink {
	int id;
	PathNode *a,*b;
	double dist;
	PathNode *getOther(PathNode *p)
	{
		return p==a ? b : a;
	}
#ifdef ENABLE_DEBUG_PATHRENDER
	bool path;
#endif
};


struct PathNode {
	int id;
	glm::vec3 position;
	int flags;
	std::vector<PathNodeLink *> links;
#ifdef ENABLE_DEBUG_PATHRENDER
	// Debug
	bool open,closed,current;
#endif
};

#endif
