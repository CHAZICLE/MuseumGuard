#ifndef __BUTTON_H_INCLUDED__
#define __BUTTON_H_INCLUDED__

#include "gui/Element.hpp"

class Button : public Element {
	private:
		glm::vec4 backgroundColor,selectedBackgroundColor;
	public:
		Button();
		~Button();
		virtual void render(double time, double fps, glm::mat4 matrix);
};

#endif
