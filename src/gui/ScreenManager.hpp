#ifndef 3YP_GUI_SCREENMANAGER_HPP_
#define 3YP_GUI_SCREENMANAGER_HPP_

class Screen;
namespace util {
class DeltaTime;
}
namespace render {
class RenderManager;
}

#include <glm/glm.hpp>
#include <list>

class ScreenManager {
	private:
		std::list<Screen *> screens;
		double lastCursorX,lastCursorY;
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
		 * Exits the screen manager
		 */
		virtual void close();
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
		 * Called by ScreenManager when the surface screen changes
		 */
		virtual void onSurfaceScreenChanged(Screen *screen);

		/**
		 * Render the screens
		 */
		void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		double getWidth();
		double getHeight();
};

#endif
