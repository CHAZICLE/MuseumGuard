#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <glm/vec3.hpp>
#include "input/ControlScheme.hpp"

namespace entities {
	class Enemy;
	class Player;
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

class World {
private:
	entities::Player *player;
	entities::Enemy *enemy;
	double vertAngle, horizAngle, lastX, lastY;
	glm::vec3 viewDirection, viewUp;
	controls::ControlScheme *controlScheme;
public:
	World();
	~World();
	void tick(util::DeltaTime *deltaTime);
	void render(render::RenderManager *manager);
};

#endif
