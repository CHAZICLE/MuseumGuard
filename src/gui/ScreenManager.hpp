#ifndef __SCREENMANAGER_H_INCLUDED__
#define __SCREENMANAGER_H_INCLUDED__

class Screen;


#include <list>

class ScreenManager {
	private:
		std::list<Screen *> screens;
	public:
		ScreenManager();
		~ScreenManager();
		
		// GUI Controls
		/**
		 * Opens a screen at the base of the stack
		 */
		void openRootScreen(Screen *screen);
		/**
		 * Opens a screen over the existing screens
		 */
		void openScreen(Screen *screen);
		/**
		 * Returns true if the screen is a surface screen
		 */
		bool isScreenSurface(Screen *screen);
		/**
		 * Returns true if the screen is at the base
		 */
		bool isScreenBase(Screen *screen);
		/**
		 * Closes the selected screen
		 */
		Screen *closeScreen(Screen *screen);
		
		// Events
		/**
		 * Render the screens
		 */
		void render(double time, double fps, glm::mat4 matrix);
};

#endif
