#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"

namespace world {
	class Entity {
		private:
			glm::vec3 position, velocity;
			glm::quat orientation;
		public:
			Entity();
			virtual ~Entity();
			glm::vec3 getPosition();
			void setPosition(glm::vec3 location);
			void translate(glm::vec3 location);
			glm::quat getOrientation();
			void setOrientation(glm::quat rotation);
			virtual void tick(util::DeltaTime  &deltaTime);
			virtual void render(render::RenderManager &rManager);
			void renderDebug(render::RenderManager &rManager, bool renderPosition, bool renderOrientation);
	};
}

#endif
