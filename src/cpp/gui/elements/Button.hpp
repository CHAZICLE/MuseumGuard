#ifndef FYP_GUI_ELEMENTS_BUTTON_HPP_
#define FYP_GUI_ELEMENTS_BUTTON_HPP_

namespace render {
	class Font;
}

#include "gui/Element.hpp"
#include <string>

class Button : public Element {
	private:
		glm::vec4 backgroundColor,selectedBackgroundColor;
		std::string text;
	public:
		Button(std::string text);
		~Button();
		virtual void render(util::DeltaTime &deltaTime, render::RenderManager &manager);
		static render::Font *buttonFont;
};

#endif
