#ifndef 3YP_GUI_GUIHOLDER_HPP_
#define 3YP_GUI_GUIHOLDER_HPP_

class GUIHolder;
class Font;
struct CursorPos;
struct DisplayMetrics;
class GLFWwindow;

#include "ui/GUI.hpp"

struct CursorPos {
	double x,y,dx,dy;
};
struct DisplayMetrics {
	double widthMM,heightMM,aspectRatio;
	int widthPx,heightPx;
};

class GUIHolder {
	public:
		GUIHolder();
		~GUIHolder();
		void runGame();
		void openRootGUI(GUI *gui);
		void openGUI(GUI *gui);
		void closeGUI(GUI *gui);
		void closeSurfaceGUI();
		void openMainMenu();//closeRootGUI();
		bool isGUISurface(GUI *gui);
		static GUIHolder *eventHandler;
		static void onError(int error, const char *msg);
		static void onKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mode);
		static void onCursorPosEvent(GLFWwindow *window, double x, double y);
		static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		static void onScrollEvent(GLFWwindow* window, double dx, double dy);
		void setMouseEnabled(bool enabled);
		struct CursorPos cursor;
		struct DisplayMetrics metrics;
		struct DisplayMetrics lastMetrics;
		GLFWwindow *window;
	private:
		GUI *firstGui,*lastGui;
		Font *font;
};

#endif
