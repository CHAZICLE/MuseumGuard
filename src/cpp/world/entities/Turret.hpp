#ifndef FYP_WORLD_ENTITIES_TURRET_HPP_
#define FYP_WORLD_ENTITIES_TURRET_HPP_

#include "world/Entity.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"
#include "ai/PerceptionManager.hpp"

namespace world {
	namespace entities {
		class Turret : public Entity {
			typedef Entity super;
			private:
				glm::vec3 targetPosition;
				render::SkeletalModel *model;
				render::SkeletalAnimation *initAnimation;
				ai::PerceptionManager *perception;
				double animationDuration,animationCurrent;
				bool initAnimating;
			public:
				Turret();
				virtual ~Turret();
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
				bool selector;
		};
	}
}

#endif
