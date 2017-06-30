#ifndef FYP_GUI_SCREENS_GAMEPAUSEMENU_HPP_
#define FYP_GUI_SCREENS_GAMEPAUSEMENU_HPP_

class Button;

#include "gui/Screen.hpp"
#include "render/Font.hpp"

class GamePauseMenu : public Screen {
	typedef Screen super;
	public:
		GamePauseMenu();
		~GamePauseMenu();
		render::Font *font;
		Button *exitButton,*resumeButton,*pathTestButton;
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &rManager);
		virtual bool onControlEvent(int control, int action);
};

#endif
