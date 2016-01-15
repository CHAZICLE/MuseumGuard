#ifndef __ELEMENT_H_INCLUDED__
#define __ELEMENT_H_INCLUDED__

class Screen;

#include <glm/glm.hpp>

class Element {
	protected:
		float x, y, width, height;
	public:
		Screen *screen;
		Element *next,*prev,*left,*right,*up,*down;
		
		Element();
		~Element();
		
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		void setX(float x);
		void setY(float y);
		void setWidth(float width);
		void setHeight(float height);
		void setSize(float x, float y, float width, float height);
		
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
