#ifndef __ENEMY_H_INCLUDED__
#define __ENEMY_H_INCLUDED__

#include "world/Entity.hpp"

namespace entities {
	class Enemy : public Entity {
		public:
			Enemy();
			virtual ~Enemy();
			virtual void render(render::RenderManager *manager);
	};
}

#endif
