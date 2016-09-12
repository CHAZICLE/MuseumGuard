#ifndef FYP_GUI_SCREENS_GAMEVIEW_HPP_
#define FYP_GUI_SCREENS_GAMEVIEW_HPP_

#include "gui/Screen.hpp"
#include "input/Controls.hpp"
#include <glm/detail/type_mat.hpp>
#include "render/Font.hpp"

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
		render::Font *statusFont;
		double pauseStartTime,pauseOffsetTime;
		bool wasSurface;
		int gameCompletionState;
	public:
		GameView();
		virtual ~GameView();
		void onGameOver(int type);
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		bool onControlEvent(Control control, int action);
		virtual void onScreenResize();
		virtual bool supportsCursor();
	};

} /* namespace screens */

#endif
