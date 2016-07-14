#ifndef FYP_INPUT_CONTROLSCHEME_HPP_
#define FYP_INPUT_CONTROLSCHEME_HPP_

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
