#ifndef __SCREENMANAGER_H_INCLUDED__
#define __SCREENMANAGER_H_INCLUDED__

class Screen;

#include <glm/glm.hpp>
#include <list>

class ScreenManager {
	private:
		std::list<Screen *> screens;
	protected:
		double width;
		double height;
	public:
		ScreenManager();
		virtual ~ScreenManager();

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
		 * On control boolean update
		 */
		virtual bool onControlEvent(int control, int action);
		/**
		 * On control delta update
		 */
		virtual bool onControlEvent(int control, double x, double y, double dx, double dy);
		/**
		 * Called by the screen manager whenever the screen resizes
		 */
		virtual void onScreenResize();

		/**
		 * Render the screens
		 */
		void render(double time, double fps, glm::mat4 matrix);
		double getWidth();
		double getHeight();
};

#endif
