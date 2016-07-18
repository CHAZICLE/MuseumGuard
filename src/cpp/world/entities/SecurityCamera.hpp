#ifndef FYP_WORLD_ENTITIES_SECURITYCAMERA_HPP_
#define FYP_WORLD_ENTITIES_SECURITYCAMERA_HPP_

#include "world/Entity.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"
#include "ai/PerceptionManager.hpp"

namespace world {
	namespace entities {
		class SecurityCamera : public Entity {
			typedef Entity super;
			private:
				render::SkeletalModel *model;
				render::SkeletalAnimation *initAnimation;
				double animationDuration,animationCurrent;
				bool initAnimating;
				ai::PerceptionManager *perception;
			public:
				SecurityCamera();
				virtual ~SecurityCamera();
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
				bool selector;
		};
	}
}

#endif
