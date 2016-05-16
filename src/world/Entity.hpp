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
		glm::quat orientation;
	public:
		Entity();
		virtual ~Entity();
		glm::vec3 getPosition();
		void setPosition(glm::vec3 location);
		void translate(glm::vec3 location);
		glm::quat getOrientation();
		void setOrientation(glm::quat rotation);
		virtual void render(render::RenderManager &manager);
};

#endif
