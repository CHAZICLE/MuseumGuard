#ifndef __SECURITYCAMERA_H_INCLUDED__
#define __SECURITYCAMERA_H_INCLUDED__

#include "world/Entity.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

namespace world {
	namespace entities {
		class SecurityCamera : public Entity {
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
				SecurityCamera();
				virtual ~SecurityCamera();
				void keepLookingAt(Entity *ent);
				virtual void tick(util::DeltaTime &deltaTime);
				virtual void render(render::RenderManager &rManager);
				bool selector;
		};
	}
}

#endif
