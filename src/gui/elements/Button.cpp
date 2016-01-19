#include <iostream>
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Button.hpp"

Button::Button()
{
	setSize(10, 0, 10, 10);
	this->selectedBackgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	this->backgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.f);
}
Button::~Button()
{
	
}
void Button::render(double time, double fps, glm::mat4 matrix)
{
	glm::mat4 boxMat = matrix;

	glUseProgram(shaders::program_solidcolor);
	boxMat = glm::translate(boxMat, glm::vec3(this->getX(), this->getY(), 0));
	boxMat = glm::scale(boxMat, glm::vec3(this->getWidth(), this->getHeight(), 0));
	
	glUniformMatrix4fv(shaders::program_solidcolor_MVP, 1, GL_FALSE, &boxMat[0][0]);
	if(this->selected)
		glUniform4fv(shaders::program_solidcolor_inColor, 1, &this->selectedBackgroundColor[0]);
	else
		glUniform4fv(shaders::program_solidcolor_inColor, 1, &this->backgroundColor[0]);
	
	BasicShapes::renderUnitSquare(shaders::program_solidcolor_vertexPosition);

	glUseProgram(shaders::program_solidcolor);
	glUniformMatrix4fv(shaders::program_solidcolor_MVP, 1, GL_FALSE, &matrix[0][0]);
	glUniform4f(shaders::program_solidcolor_inColor, 0.5f, 0.f, 0.f, 1.f);
	BasicShapes::drawLine(glm::vec3(0,0,0),glm::vec3(10,10,0),shaders::program_solidcolor_vertexPosition);
}
