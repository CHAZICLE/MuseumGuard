#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

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
