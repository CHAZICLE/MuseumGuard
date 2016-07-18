#ifndef FYP_GUI_SCREENS_GAMEPAUSEMENU_HPP_
#define FYP_GUI_SCREENS_GAMEPAUSEMENU_HPP_

class Button;

#include "gui/Screen.hpp"

class GamePauseMenu : public Screen {
	public:
		GamePauseMenu();
		~GamePauseMenu();
		Button *exitButton,*resumeButton,*pathTestButton;
		virtual bool onControlEvent(int control, int action);
};

#endif
