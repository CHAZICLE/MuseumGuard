#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <glm/vec3.hpp>
#include <list>
#include "input/ControlScheme.hpp"

namespace world {
	class World;
	namespace entities {
		class Enemy;
		class Player;
	}
}
namespace render {
	class RenderManager;
}
namespace util {
	class DeltaTime;
}
namespace controls {
	class ControlScheme;
}

namespace world {
	class World {
	private:
		std::list<world::Entity *> entities;
		world::entities::Player *player;
		world::entities::Enemy *enemy;
		double vertAngle, horizAngle, lastX, lastY;
		glm::vec3 viewDirection, viewUp;
		controls::ControlScheme *controlScheme;
	public:
		World();
		~World();
		void tick(util::DeltaTime &deltaTime, bool surface);
		void render(render::RenderManager &manager);
	};
}

#endif
