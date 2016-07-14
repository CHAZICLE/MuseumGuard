#ifndef FYP_GUI_ELEMENT_HPP_
#define FYP_GUI_ELEMENT_HPP_
namespace render {
class RenderManager;
} /* namespace render */
namespace util {
class DeltaTime;
} /* namespace util */

class Screen;

#include <glm/glm.hpp>

class Element {
	private:
	protected:
		float x, y, width, height;
	public:
		Screen *screen;
		Element *next,*prev,*left,*right,*up,*down;
		bool selected;
		
		Element();
		virtual ~Element();
		
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		Element *setX(float x);
		Element *setY(float y);
		Element *setWidth(float width);
		Element *setHeight(float height);
		Element *setSize(float x, float y, float width, float height);
		bool isInside(float x, float y);
		
		/**
		 * Renders the GUI screen
		 */
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		/**
		 * Called by resize methods
		 */
		virtual void onElementResize();
		
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
