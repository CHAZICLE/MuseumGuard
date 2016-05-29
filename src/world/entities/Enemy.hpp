#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "world/Entity.hpp"

namespace world {
	namespace entities {
		class Enemy : public Entity {
			public:
				Enemy();
				virtual ~Enemy();
				virtual void render(render::RenderManager &rManager);
		};
	}
}

#endif
