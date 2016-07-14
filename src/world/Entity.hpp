#ifndef 3YP_WORLD_ENTITY_HPP_
#define 3YP_WORLD_ENTITY_HPP_

namespace world {
	class World;
}

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "util/Boundaries.hpp"

namespace world {
	class Entity {
		private:
			glm::vec3 position, velocity;
			glm::quat orientation;
			World *world;
			double spawnTime;
		protected:
			util::Boundaries::AABB *bounds;
		public:
			bool doRender;

			Entity();
			virtual ~Entity();
			virtual void addedToWorld(world::World *world, double spawnTime);
			world::World &getWorld();
			double getSpawnTime();

			// Position and rotation functions
			glm::vec3 getPosition();
			void setPosition(glm::vec3 location);
			void translate(glm::vec3 location);
			glm::quat getOrientation();
			void setOrientation(glm::quat rotation);

			util::Boundaries::AABB *getBounds();

			virtual void tick(util::DeltaTime  &deltaTime);
			virtual void render(render::RenderManager &rManager);
			void renderDebug(render::RenderManager &rManager, bool renderPositionMarker, bool renderBounds);
	};
}

#endif
