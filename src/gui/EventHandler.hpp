#ifndef 3YP_GUI_EVENTHANDLER_HPP_
#define 3YP_GUI_EVENTHANDLER_HPP_

class EventHandler {
	public:
		EventHandler();
		~EventHandler();
		/**
		 * On control boolean update
		 */
		bool onControlEvent(int control, bool state);
		/**
		 * On control delta update
		 */
		bool onControlEvent(int control, double x, double y, double dx, double dy);
		/**
		 * Called when the screen manager resizes
		 */
		void onResize();
};

#endif
