#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace render {
	class RenderManager;
}

class Entity {
	private:
		glm::vec3 position, velocity;
		glm::quat rotation;
	public:
		Entity();
		~Entity();
		glm::vec3 getPosition();
		glm::quat getRotation();
		void teleportTo(glm::vec3 location);
		void rotateTo(glm::quat rotation);
		void render(render::RenderManager *manager);
};

#endif
