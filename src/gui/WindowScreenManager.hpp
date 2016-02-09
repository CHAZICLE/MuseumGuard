#ifndef __WINDOWSCREENMANAGER_H_INCLUDED__
#define __WINDOWSCREENMANAGER_H_INCLUDED__

struct GLFWwindow;

#include "ScreenManager.hpp"

class WindowScreenManager : public ScreenManager {
	private:
		GLFWwindow *window;
		int lastWindowWidthPx,lastWindowHeightPx,windowWidthPx,windowHeightPx,monitorWidthPx,monitorHeightPx,modeWidthPx,modeHeightPx;
		double scale;
		double lastX,lastY;
	public:
		WindowScreenManager();
		virtual ~WindowScreenManager();
		static WindowScreenManager *eventHandler;
		virtual void close();
		static void onError(int error, const char *msg);
		static void onKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mode);
		static void onCursorPosEvent(GLFWwindow *window, double x, double y);
		static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		static void onScrollEvent(GLFWwindow* window, double dx, double dy);
		void run();
};

#endif
