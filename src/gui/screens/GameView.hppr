#ifndef __GAMEVIEW_HPP__
#define __GAMEVIEW_HPP__

#include <glm/detail/type_mat.hpp>
#include "../Screen.hpp"

class World;
namespace util {
	class DeltaTime;
}
namespace render {
	class RenderManager;
}

namespace screens {

	class GameView: public Screen {
	private:
		World *world;
	public:
		GameView();
		virtual ~GameView();
		virtual void render(util::DeltaTime *deltaTime, render::RenderManager *manager);
		bool onControlEvent(int control, int action);
		virtual void onScreenResize();
		virtual bool supportsCursor();
	};

} /* namespace screens */

#endif /* SRC_GUI_SCREENS_GAMEVIEW_HPP_ */
