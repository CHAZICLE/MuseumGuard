#ifndef __TURRET_H_INCLUDED__
#define __TURRET_H_INCLUDED__

#include "world/Entity.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

namespace world {
	namespace entities {
		class Turret : public Entity {
			typedef Entity super;
			private:
				float currentPitch,currentYaw,targetPitch,targetYaw;
				Entity *targetEntity;
				glm::vec3 targetPosition;
				render::SkeletalModel *model;
				render::SkeletalAnimation *initAnimation;
				double animationDuration,animationCurrent;
				bool initAnimating;
			public:
				Turret();
				virtual ~Turret();
				void keepLookingAt(Entity *ent);
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
				bool selector;
		};
	}
}

#endif
