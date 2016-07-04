#ifndef __PATHHOLDER_H_INCLUDED__
#define __PATHHOLDER_H_INCLUDED__

#include "PathCommon.h"

#include <vector>

#include "util/AssetManager.hpp"

#include "render/RenderManager.hpp"


namespace ai {
	namespace path {
		class NavigationGraph : public util::Asset {
			public:
				int numNodes;
				PathNode **nodes;
				NavigationGraph(int assetId, std::istream &fp);
				virtual ~NavigationGraph();
				virtual void write(std::ostream &ost) const;
				virtual void postload();
				void render(render::RenderManager &manager);
		};
}
}

#endif
