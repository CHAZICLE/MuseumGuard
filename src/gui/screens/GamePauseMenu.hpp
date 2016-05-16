#ifndef __GAMEPAUSEMENU_H_INCLUDED__
#define __GAMEPAUSEMENU_H_INCLUDED__

class Button;

#include "gui/Screen.hpp"

class GamePauseMenu : public Screen {
	public:
		GamePauseMenu();
		~GamePauseMenu();
		Button *exitButton,*playButton,*pathTestButton;
		virtual bool onControlEvent(int control, int action);
};

#endif
