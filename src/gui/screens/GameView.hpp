#ifndef 3YP_GUI_SCREENS_GAMEVIEW_HPP_
#define 3YP_GUI_SCREENS_GAMEVIEW_HPP_

#include "gui/Screen.hpp"
#include "input/Controls.hpp"
#include <glm/detail/type_mat.hpp>

namespace world {
	class World;
}
namespace util {
	class DeltaTime;
}
namespace render {
	class RenderManager;
}

namespace screens {

	class GameView: public Screen {
	private:
		world::World *world;
		double pauseStartTime,pauseOffsetTime;
		bool wasSurface;
	public:
		GameView();
		virtual ~GameView();
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		bool onControlEvent(Control control, int action);
		virtual void onScreenResize();
		virtual bool supportsCursor();
	};

} /* namespace screens */

#endif
