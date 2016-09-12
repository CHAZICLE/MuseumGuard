#ifndef FYP_WORLD_ENTITIES_ENEMY_HPP_
#define FYP_WORLD_ENTITIES_ENEMY_HPP_

#include "world/Entity.hpp"

//#include "ai/ObjectiveManager.hpp"
namespace ai {
	class ObjectiveManager;
}
#include "ai/path/PathExecuter.hpp"

namespace world {
	namespace entities {
		typedef Entity super;
		class Enemy : public Entity {
			ai::ObjectiveManager *objectiveManager;
			double animTime;
			public:
				Enemy(world::World *world);
				virtual ~Enemy();
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
				virtual void die(double time, glm::vec3 direction, int type);
				void setPath(std::vector<int> path);
		};
	}
}

#endif
