#ifndef __BOUNDARIES_H_INCLUDED__
#define __BOUNDARIES_H_INCLUDED__

#include "render/RenderManager.hpp"

namespace util {
	namespace Boundaries {
		struct Sphere {
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
		class OBB {
			public:
				glm::vec3 min,max;
				OBB(const glm::vec3 &min, const glm::vec3 &max);
				OBB(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ);
		};
		class AABB {
			public:
				float boxCenter[3];
				float boxHalfSize[3];
				AABB();
				AABB(const float boxCenterX, const float boxCenterY, const float boxCenterZ, const float boxHalfSizeX, const float boxHalfSizeY, const float boxHalfSizeZ);
				AABB(const glm::vec3 &min, const glm::vec3 &max);
				~AABB();
				AABB translate(const glm::vec3 &offset);
				OBB rotate(const glm::quat &rotation);
				bool checkInside(const glm::vec3 v);
				bool checkIntersect(const AABB &aabb);
				bool checkIntersect(const Sphere &rbb);
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
