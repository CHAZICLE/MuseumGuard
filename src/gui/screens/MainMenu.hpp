#ifndef __MAINMENU_H_INCLUDED__
#define __MAINMENU_H_INCLUDED__

class Button;

#include "gui/Screen.hpp"

class MainMenu : public Screen {
	public:
		MainMenu();
		~MainMenu();
		Button *exitButton,*playButton;
		virtual bool onControlEvent(int control, int action);
};

#endif
