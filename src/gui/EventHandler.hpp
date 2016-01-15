#ifndef __EVENTHANDLER_H_INCLUDED__
#define __EVENTHANDLER_H_INCLUDED__

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
