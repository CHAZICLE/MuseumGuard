#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <glm/vec3.hpp>

namespace entities {
class Enemy;
class Player;
} /* namespace entities */
namespace render {
class RenderManager;
} /* namespace render */
namespace util {
class DeltaTime;
} /* namespace util */

class World {
private:
	entities::Player *player;
	entities::Enemy *enemy;
	double vertAngle, horizAngle, lastX, lastY;
	glm::vec3 viewDirection, viewUp;
public:
	World();
	~World();
	void tick(util::DeltaTime *deltaTime);
	void render(render::RenderManager *manager);
};

#endif
