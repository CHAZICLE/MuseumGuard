#ifndef __DEBUGCONTROLS_H_INCLUDED__
#define __DEBUGCONTROLS_H_INCLUDED__

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
