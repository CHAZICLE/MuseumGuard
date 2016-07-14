#ifndef 3YP_GUI_SCREENS_MAINMENU_HPP_
#define 3YP_GUI_SCREENS_MAINMENU_HPP_

class Button;

#include "gui/Screen.hpp"
#include "input/Controls.hpp"

class MainMenu : public Screen {
	public:
		MainMenu();
		virtual ~MainMenu();
		Button *exitButton,*playButton,*pathTestButton;
		virtual bool onControlEvent(Control control, int action);
};

#endif
