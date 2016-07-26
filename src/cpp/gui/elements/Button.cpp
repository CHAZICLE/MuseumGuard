#include <iostream>
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/RenderManager.hpp"
#include "render/Font.hpp"

#include "Button.hpp"

#define RGBf(r,g,b) r/255.f, g/255.f, b/255.f

using namespace render;

render::Font *Button::buttonFont = 0;
Button::Button(std::string text) : Element()
{
	setSize(0, 0, 60, 10);
	this->selectedBackgroundColor = glm::vec4(RGBf(140, 167, 246), 1.f);
	this->backgroundColor = glm::vec4(RGBf(84, 106, 171), 1.f);
	this->text = text;
	if(this->buttonFont==0)
	{
		this->buttonFont = new render::Font("cour.ttf", 8);
	}
}
Button::~Button()
{
	
}
void Button::render(util::DeltaTime &deltaTime, render::RenderManager &rManager)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->getX(), this->getY(), 0));
	glm::mat4 boxMat;

	// Render the background box
	boxMat = translationMatrix*glm::scale(boxMat, glm::vec3(this->getWidth(), this->getHeight(), 0));
	
	rManager.M = boxMat;
	rManager.markMDirty();
	shaders::ShaderProgram *prog = rManager.useShader(SHADER_solidColor);
	GLint loc = prog->getShaderLocation(true, SHADER_solidColor_solidColor);
	GLint vploc = rManager.getVertexPosition();

	if(this->selected)
		glUniform4fv(loc, 1, &this->selectedBackgroundColor[0]);
	else
		glUniform4fv(loc, 1, &this->backgroundColor[0]);
	
	BasicShapes::renderUnitSquare(vploc);

	//// Render debug line
	//rManager.M = translationMatrix;
	//rManager.markMDirty();

	//glUniform4f(loc, 0.5f, 0.f, 0.f, 1.f);
	//BasicShapes::drawLine(glm::vec3(0,0,0),glm::vec3(10,10,0),vploc);

	// Render text
	boxMat = glm::mat4(1.0f);
	boxMat = glm::translate(boxMat, glm::vec3(this->getX()+1.0f, this->getY()+3.0f, 0));
	rManager.M = boxMat;
	rManager.markMDirty();
	this->buttonFont->printf(this->text, rManager);
}
