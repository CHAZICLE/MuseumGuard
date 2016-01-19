#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__

class ScreenManager;
class Element;

#include <glm/glm.hpp>
#include <list>

class Screen {
	protected:
		Element *selectedElement;
		std::list<Element *> elements;
		bool elementSelectedWithMouse;
		Element *firstNext,*firstPrev,*firstLeft,*firstRight,*firstUp,*firstDown;
	public:
		ScreenManager *manager;
		
		Screen();
		virtual ~Screen();

		// Misc
		/**
		 * Adds an element to the screen
		 */
		void addElement(Element *e);
		/**
		 * Renders the GUI screen
		 */
		virtual void render(double time, double fps, glm::mat4 matrix);
		void selectElement(Element *element, bool mouseSelection);

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
};

#endif
