#ifndef FYP_INPUT_CONTROLS_DEBUGCONTROLS_HPP_
#define FYP_INPUT_CONTROLS_DEBUGCONTROLS_HPP_

namespace world {
	class Entity;
}

#include "input/ControlScheme.hpp"

namespace controls {
	class DebugControls : public ControlScheme {
		private:
			double lastCursorX,lastCursorY,cursorX,cursorY,cursorDeltaX,cursorDeltaY;
			bool lastCursor;
		public:
			DebugControls(world::Entity *entity);
			virtual ~DebugControls();
			virtual void tick(util::DeltaTime &deltaTime);
	};
}

#endif
