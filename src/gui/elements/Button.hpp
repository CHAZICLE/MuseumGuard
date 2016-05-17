#ifndef __BUTTON_H_INCLUDED__
#define __BUTTON_H_INCLUDED__

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
