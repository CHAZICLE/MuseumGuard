#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__

#define CONTROL_GUI_NEXT 0
#define CONTROL_GUI_PREV 1
#define CONTROL_GUI_UP 2
#define CONTROL_GUI_DOWN 3
#define CONTROL_GUI_LEFT 4
#define CONTROL_GUI_RIGHT 5

class ScreenManager;
class Element;

#include <glm/glm.hpp>
#include <list>

class Screen {
	protected:
		Element *selectedElement;
		std::list<Element *> elements;
		Element *firstNext,*firstPrev,*firstLeft,*firstRight,*firstUp,*firstDown;
	public:
		ScreenManager *manager;
		
		Screen();
		~Screen();
		
		void addElement(Element *e);
		
		/**
		 * On control boolean update
		 */
		virtual bool onControlEvent(int control, bool state);
		/**
		 * On control delta update
		 */
		virtual bool onControlEvent(int control, double x, double y, double dx, double dy);
		/**
		 * Called by the screen manager whenever the screen resizes
		 */
		virtual void onResize();
		/**
		 * Renders the GUI screen
		 */
		virtual void render(double time, double fps, glm::mat4 matrix);
		
};

#endif
