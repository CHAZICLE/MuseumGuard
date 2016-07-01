#ifndef __CONTROLSCHEME_H_INCLUDED__
#define __CONTROLSCHEME_H_INCLUDED__

namespace util {
	class DeltaTime;
}
namespace world {
	class Entity;
}

#include <glm/glm.hpp>

namespace controls {
	class ControlScheme {
		protected:
			world::Entity *controlEntity;
		public:
			ControlScheme(world::Entity *controlEntity);
			virtual ~ControlScheme();
			virtual void tick(util::DeltaTime &deltaTime);
	};
};

#endif
