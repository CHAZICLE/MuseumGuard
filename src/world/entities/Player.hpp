#ifndef 3YP_WORLD_ENTITIES_PLAYER_HPP_
#define 3YP_WORLD_ENTITIES_PLAYER_HPP_

#include "world/Entity.hpp"

namespace world {
	namespace entities {
		typedef Entity super;
		class Player : public Entity {
			public:
				Player();
				~Player();
		};
	}
}

#endif
