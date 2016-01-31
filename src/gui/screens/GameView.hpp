#ifndef SRC_GUI_SCREENS_GAMEVIEW_HPP_
#define SRC_GUI_SCREENS_GAMEVIEW_HPP_

#include <glm/detail/type_mat.hpp>

#include "../Screen.hpp"

class World;

namespace screens {

class GameView: public Screen {
private:
	World *world;
public:
	GameView();
	virtual ~GameView();
	virtual void render(double time, double fps, glm::mat4 matrix);
	virtual void onScreenResize();
	virtual bool supportsCursor();
};

} /* namespace screens */

#endif /* SRC_GUI_SCREENS_GAMEVIEW_HPP_ */
