#ifndef 3YP_INPUT_CONTROLS_PLAYERGAMECONTROLS_HPP_
#define 3YP_INPUT_CONTROLS_PLAYERGAMECONTROLS_HPP_

namespace world {
	class Entity;
}

#include "input/ControlScheme.hpp"
#include "world/collisions/StaticMesh.hpp"

namespace controls {
	class PlayerGameControls : public ControlScheme {
		private:
			double lastCursorX,lastCursorY,cursorX,cursorY,cursorDeltaX,cursorDeltaY;
			bool lastCursor;
			world::collisions::StaticMesh *world;
			glm::vec3 velocity;
		public:
			PlayerGameControls(world::Entity *entity, world::collisions::StaticMesh *staticMesh);
			virtual ~PlayerGameControls();
			//virtual void tick(util::DeltaTime &deltaTime);
			void tick2(render::RenderManager *rManager, util::DeltaTime &deltaTime);
	};
}

#endif
