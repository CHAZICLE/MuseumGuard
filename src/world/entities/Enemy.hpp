#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "world/Entity.hpp"

#include "ai/ObjectiveManager.hpp"
#include "ai/path/PathExecuter.hpp"

namespace world {
	namespace entities {
		typedef Entity super;
		class Enemy : public Entity {
			ai::ObjectiveManager *objectiveManager;
			ai::path::PathExecuter *pathExecuter;
			double animTime;
			public:
				Enemy();
				virtual ~Enemy();
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
		};
	}
}

#endif
