#ifndef __BOUNDARIES_H_INCLUDED__
#define __BOUNDARIES_H_INCLUDED__

#include "render/RenderManager.hpp"

namespace util {
	namespace Boundaries {
		struct RBB {
			glm::vec3 center;
			float radius;
		};
		struct Raycast {
			glm::vec3 origin;
			glm::vec3 direction;
			float maxDistance;
		};
		struct RaycastResult {
			float distance;
			glm::vec3 hitNormal;
			bool hit;
		};

		class AABB {
			public:
				float boxCenter[3];
				float boxHalfSize[3];
				AABB();
				AABB(float boxCenterX, float boxCenterY, float boxCenterZ, float boxHalfSizeX, float boxHalfSizeY, float boxHalfSizeZ);
				~AABB();
				static AABB *fromMinMax(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ);
				AABB *translate(glm::vec3 offset);
				bool checkInside(const glm::vec3 v);
				bool checkIntersect(const AABB &aabb);
				bool checkIntersect(const RBB &rbb);
				void render(render::RenderManager &rManager, glm::vec4 color, bool solid);
				float rayCastDistance(Raycast &raycast);
				bool rayCastCheck(Raycast &raycast);
				RaycastResult *rayCast(Raycast &raycast);
				glm::vec3 min();
				glm::vec3 max();
				float minX();
				float minY();
				float minZ();
				float maxX();
				float maxY();
				float maxZ();
		};
	}
}

#endif
