#ifndef __MAINMENU_H_INCLUDED__
#define __MAINMENU_H_INCLUDED__

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
