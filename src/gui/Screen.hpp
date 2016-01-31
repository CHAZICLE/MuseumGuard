#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__

#include <glm/detail/type_mat.hpp>
#include <list>

#include "Element.hpp"

class ScreenManager;

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
		virtual void render(util::DeltaTime *deltaTime, render::RenderManager *manager);
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

		virtual bool supportsCursor();
};

#endif
