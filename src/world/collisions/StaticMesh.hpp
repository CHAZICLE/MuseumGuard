#ifndef 3YP_WORLD_COLLISIONS_STATICMESH_HPP_
#define 3YP_WORLD_COLLISIONS_STATICMESH_HPP_

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
				StaticMeshCell rootCell;
			protected:

				// Adds a triangle to a cell and its children if needed
				void addTriangleToCell(StaticMeshTriangle &triangle, StaticMeshCell &cell, int cellLevel);
				// Adds a triangle to a cells children
				void addTriangleToCellChildren(StaticMeshTriangle &triangle, StaticMeshCell &parent, int childrenLevel);

				// Divides a cell into pieces
				void subdivideCell(StaticMeshCell &parent, int childrenLevel);
				// Adds a child cell to parent cell
				void addChildToCell(StaticMeshCell &parent, int childIndex, util::Boundaries::AABB *boxBounds);

				///////////////////////////////////
				// Collision Detection Functions //
				///////////////////////////////////
				
				// Raycasting functions
				StaticMeshTriangle *rayCastCellChildren(util::Boundaries::Raycast &raycast, StaticMeshCell &cell, util::Boundaries::RaycastResult &result);
				StaticMeshTriangle *rayCastCellTriangles(util::Boundaries::Raycast &raycast, StaticMeshCell &cell, util::Boundaries::RaycastResult &result);
			public:
				StaticMesh();
				~StaticMesh();
				//## Misc utils
				glm::vec3 getTriangleNormal(const StaticMeshTriangle &triangle);
				
				// Adds a static model to the static mesh collision checker
				void addStaticModel(render::StaticModel *model);

				// debug
				void render(render::RenderManager &rManager, util::Boundaries::AABB &selectionBox);
				void renderCellChildren(render::RenderManager &rManager, util::Boundaries::AABB &selectionBox, StaticMeshCell &cell);
				void render(render::RenderManager &rManager, util::Boundaries::Raycast &raycast);
				StaticMeshTriangle *renderCellChildren(render::RenderManager &rManager, util::Boundaries::Raycast &raycast, StaticMeshCell &cell, util::Boundaries::RaycastResult &result);

				///////////////////////////////////
				// Collision Detection Functions //
				///////////////////////////////////

				// Raycasting functions
				util::Boundaries::RaycastResult rayCast(util::Boundaries::Raycast &raycast);

				// Intersection functions
				bool collisionResponse(render::RenderManager &rManager, const util::Boundaries::AABB &aabb, glm::vec3 *velocity);
				bool collisionResponseCellChildren(render::RenderManager &rManager, const util::Boundaries::AABB &aabb, const StaticMeshCell &parent, glm::vec3 *velocity);

				bool collisionResponse(const util::Boundaries::Sphere &sphere, glm::vec3 *step, glm::vec3 *velocity);
				bool collisionResponseCellChildren(const util::Boundaries::Sphere &sphere, const StaticMeshCell &parent, glm::vec3 *step, glm::vec3 *velocity);
				bool collisionResponseCellTriangles(const util::Boundaries::Sphere &sphere, const StaticMeshCell &cell, glm::vec3 *step, glm::vec3 *velocity);
				bool collisionResponseCellTriangleEdge(const util::Boundaries::Sphere &sphere, glm::vec3 *step, glm::vec3 *velocity, const glm::vec3 &V0, const glm::vec3 &V1, const glm::vec3 &P1);
				bool collisionResponseCellTriangleVertex(const util::Boundaries::Sphere &sphere, const glm::vec3 &P1, const glm::vec3 &V, float &enter, float &exit);
		};
	}
}

#endif
