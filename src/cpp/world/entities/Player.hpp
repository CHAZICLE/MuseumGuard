#ifndef FYP_WORLD_ENTITIES_PLAYER_HPP_
#define FYP_WORLD_ENTITIES_PLAYER_HPP_

#include "world/Entity.hpp"

namespace world {
	namespace entities {
		typedef Entity super;
		class Player : public Entity {
			public:
				Player();
				~Player();
				virtual void die(double time, glm::vec3 direction, int type);
		};
	}
}

#endif
