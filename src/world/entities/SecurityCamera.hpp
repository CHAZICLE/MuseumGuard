#ifndef __SECURITYCAMERA_H_INCLUDED__
#define __SECURITYCAMERA_H_INCLUDED__

#include "world/Entity.hpp"

namespace world {
	namespace entities {
		class SecurityCamera : public Entity {
			public:
				SecurityCamera();
				~SecurityCamera();
				void keepLookingAt(Entity *ent);
				virtual void render(render::RenderManager &rManager);
			private:
				float pitch,yaw;
				Entity *trackingEntity;
		};
	}
}

#endif
