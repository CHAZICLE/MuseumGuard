#ifndef __STATICMESH_H_INCLUDED__
#define __STATICMESH_H_INCLUDED__

namespace world {
	namespace collisions {
		class StaticMesh;
		typedef struct StaticMeshTriangle StaticMeshTriangle;
//typedef struct StaticMeshCell StaticMeshCell;
	}
}

#include <vector>
#include "util/Boundaries.hpp"
#include "render/StaticModel.hpp"

//debug
#include "render/RenderManager.hpp"
#include "render/BasicShapes.hpp"

namespace world {
	namespace collisions {
		struct StaticMeshCell {
			util::Boundaries::AABB *bounds;
			int level;
			bool leaf;
			std::vector<struct StaticMeshCell> children;
			std::vector<struct StaticMeshTriangle> triangles;
		};
		struct StaticMeshTriangle {
			render::StaticModel *model;
			render::StaticModelObject *modelObject;
			int triangleIndex;
		};
		class StaticMesh {
			private:
				StaticMeshCell rootNode;
			public:
				StaticMesh();
				~StaticMesh();
				//## Misc utils
				
				// Adds a static model to the static mesh collision checker
				void addStaticModel(render::StaticModel *model);
				// Adds a triangle to a cell and its children if needed
				void addTriangleToCell(StaticMeshTriangle &triangle, StaticMeshCell &cell, int cellLevel);
				// Adds a triangle to a cells children
				void addTriangleToCellChildren(StaticMeshTriangle &triangle, StaticMeshCell &parent, int childrenLevel);

				// Divides a cell into pieces
				void subdivideCell(StaticMeshCell &parent, int childrenLevel);
				// Adds a child cell to parent cell
				void addChildToCell(StaticMeshCell &parent, int childIndex, util::Boundaries::AABB *boxBounds);

				// debug
				void render(render::RenderManager &rManager, util::Boundaries::AABB &selectionBox);
				void renderCellChildren(render::RenderManager &rManager, util::Boundaries::AABB &selectionBox, StaticMeshCell *cell);

				// Raycasting functions
				util::Boundaries::RaycastResult *rayCast(util::Boundaries::Raycast &raycast);
				StaticMeshTriangle *rayCastCellChildren(util::Boundaries::Raycast &raycast, StaticMeshCell &cell, util::Boundaries::RaycastResult &result);
				StaticMeshTriangle *rayCastCellTriangles(util::Boundaries::Raycast &raycast, StaticMeshCell &cell, util::Boundaries::RaycastResult &result);
		};
	}
}

#endif
