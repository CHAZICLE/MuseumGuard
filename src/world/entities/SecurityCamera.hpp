#ifndef __SECURITYCAMERA_H_INCLUDED__
#define __SECURITYCAMERA_H_INCLUDED__

#include "world/Entity.hpp"

namespace world {
	namespace entities {
		class SecurityCamera : public Entity {
			public:
				SecurityCamera();
				~SecurityCamera();
				void setTarget(glm::vec3 target);
				virtual void render(render::RenderManager &rManager);
			private:
				glm::quat targetRotation;
				glm::vec3 targetPosition;
		};
	}
}

#endif
