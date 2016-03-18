#ifndef __DEBUGCONTROLS_H_INCLUDED__
#define __DEBUGCONTROLS_H_INCLUDED__

#include "input/ControlScheme.hpp"

namespace controls {
	class DebugControls : public ControlScheme {
		private:
			double lastCursorX,lastCursorY,cursorX,cursorY,cursorDeltaX,cursorDeltaY;
			bool lastCursor;
		public:
			DebugControls(Entity *entity);
			~DebugControls();
			virtual void tick(util::DeltaTime *deltaTime);
	};
}

#endif
