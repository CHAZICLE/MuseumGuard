#include <iostream>
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/RenderManager.hpp"

#include "Button.hpp"

Button::Button()
{
	setSize(0, 0, 10, 10);
	this->selectedBackgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	this->backgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.f);
}
Button::~Button()
{
	
}
void Button::render(util::DeltaTime *deltaTime, render::RenderManager *manager)
{
	glm::mat4 boxMat;

	glUseProgram(shaders::program_solidcolor);
	boxMat = glm::translate(boxMat, glm::vec3(this->getX(), this->getY(), 0));
	boxMat = glm::scale(boxMat, glm::vec3(this->getWidth(), this->getHeight(), 0));
	
	manager->M = boxMat;
	manager->markMDirty();
	manager->setMVPMatrix(shaders::program_solidcolor_MVP);

	if(this->selected)
		glUniform4fv(shaders::program_solidcolor_inColor, 1, &this->selectedBackgroundColor[0]);
	else
		glUniform4fv(shaders::program_solidcolor_inColor, 1, &this->backgroundColor[0]);
	
	BasicShapes::renderUnitSquare(shaders::program_solidcolor_vertexPosition);

	glUseProgram(shaders::program_solidcolor);
	manager->M = glm::mat4(1.0f);
	manager->markMDirty();
	manager->setMVPMatrix(shaders::program_solidcolor_MVP);
	glUniform4f(shaders::program_solidcolor_inColor, 0.5f, 0.f, 0.f, 1.f);
	BasicShapes::drawLine(glm::vec3(0,0,0),glm::vec3(10,10,0),shaders::program_solidcolor_vertexPosition);
}
