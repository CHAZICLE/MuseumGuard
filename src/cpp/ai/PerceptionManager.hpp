#ifndef FYP_AI_PERCEPTIONMANAGER_HPP_
#define FYP_AI_PERCEPTIONMANAGER_HPP_

#include "world/Entity.hpp"
#include "util/DeltaTime.hpp"
#include <set>

/*
 * Enemy: Turret, Player
 * Turret: Enemy
 * SecurityCamera: Enemy
 */

namespace ai {
	class PerceptionManager;
	class PerceptionManager {
		protected:
			//current = the current orientation, target = the target orientation, min/max = boundaries; all in model space
			float currentYaw,currentPitch,targetYaw,targetPitch,maxYaw,minYaw;
			world::Entity *targetEntity;
			glm::vec3 offset_modelSpace;
			world::Entity *controlEntity;
			std::set<const std::type_info *> searchTypes;
		public:
			PerceptionManager(world::Entity *controlEntity, std::initializer_list<const std::type_info *> searchTypes, glm::vec3 offset_modelSpace);
			~PerceptionManager();
			void setYawBounds(float minYaw, float maxYaw);
			void tick(util::DeltaTime &deltaTime);
			world::Entity *getTargetEntity();
			glm::quat getOrientation();
	};
}

#endif
