#include "util/Globals.hpp"
#include "util/gl.h"
#include "shaders/ShaderProgram.hpp"
#include "render/BasicShapes.hpp"

#include "RenderManager.hpp"

using namespace render;
using namespace shaders;

RenderManager::RenderManager() {
	this->mDirty = false;
	this->vDirty = false;
	this->pDirty = false;
	this->mvDirty = false;
	this->vpDirty = false;
	this->mvpDirty = false;
	this->doCullFace = false;
	this->doDepthBuffer = false;
}

RenderManager::~RenderManager() {
}

void RenderManager::setMVPMatrix(GLuint mvpMatrixShaderLocation) {
	if(this->mvpDirty)
	{
		MVP = P*V*M;
		this->mvpDirty = false;
	}
	glUniformMatrix4fv(mvpMatrixShaderLocation, 1, GL_FALSE, &MVP[0][0]);
}

void RenderManager::markPDirty() {
	this->pDirty = true;
	this->vpDirty = true;
	this->mvpDirty = true;
}

void RenderManager::markVDirty() {
	this->vDirty = true;
	this->mvDirty = true;
	this->vpDirty = true;
	this->mvpDirty = true;
}

void RenderManager::markMDirty() {
	this->mDirty = true;
	this->mvDirty = true;
	this->mvpDirty = true;
}

void RenderManager::pushMatrixM()
{
	this->stackM = this->M;
	this->stackMV = this->MV;
	this->stackMVP = this->MVP;
}
void RenderManager::popMatrixM()
{
	
	this->M = this->stackM;
	this->MV = this->stackMV;
	this->MVP = this->stackMVP;
}

#define MATRIX_SHADER_INJECT(INTVAR, MATVAR) loc = shaderProgram.getShaderLocation(true, INTVAR); if(loc!=-1) glUniformMatrix4fv(loc, 1, GL_FALSE, &MATVAR[0][0])

// Shaders
void RenderManager::setShaderMatricies(ShaderProgram &shaderProgram)
{
	if(this->mvpDirty)
	{
		MVP = P*V*M;
		this->mvpDirty = false;
	}
	GLint loc;
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_M, M);
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_V, V);
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_P, P);
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_MV, MV);
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_VP, VP);
	MATRIX_SHADER_INJECT(SHADERVAR_matrix_MVP, MVP);
}
shaders::ShaderProgram *RenderManager::useShader(int shader)
{
	this->shader = shaders::ShaderProgram::getShader(shader);
	if(this->shader==0)
	{
		util::Globals::fatalError("Selected invalid shader");
	}
	this->shader->useShader();
	setShaderMatricies(*this->shader);
	return this->shader;
}
GLint RenderManager::getVertexPosition()
{
	return this->shader->getShaderLocation(false, SHADERVAR_vertex_position);
}
GLint RenderManager::getVertexNormal()
{
	return this->shader->getShaderLocation(false, SHADERVAR_vertex_normal);
}
GLint RenderManager::getVertexTexture()
{
	return this->shader->getShaderLocation(false, SHADERVAR_vertex_texture);
}

void RenderManager::enableDepth() {
	this->doDepthBuffer = true;
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}
void RenderManager::disableDepth() {
	glDisable(GL_DEPTH_TEST);
}
void RenderManager::enableCullFace() {
	if(!this->doCullFace)
	{
		this->doCullFace = true;
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
}
void RenderManager::disableCullFace() {
	if(this->doCullFace)
	{
		this->doCullFace = false;
		glDisable(GL_CULL_FACE);
	}
}
void RenderManager::enableTransparency() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void RenderManager::disableTransparency() {
	glDisable(GL_BLEND);
}

void RenderManager::setDimensionsPx(int widthPx, int heightPx)
{
	this->widthPx = widthPx;
	this->heightPx = heightPx;
}
void RenderManager::setDimensionsMM(float widthMM, float heightMM)
{
	this->widthMM = widthMM;
	this->heightMM = heightMM;
}
int RenderManager::getWidthPx() {
	return this->widthPx;
}
int RenderManager::getHeightPx() {
	return this->heightPx;
}
float RenderManager::getWidthMM() {
	return this->widthMM;
}
float RenderManager::getHeightMM() {
	return this->heightMM;
}

void RenderManager::renderDirectionVector(const glm::vec3 position, const glm::vec3 direction, const glm::vec4 color)
{
	this->pushMatrixM();
	this->M = glm::mat4(1.0f);
	this->markMDirty();
	render::shaders::ShaderProgram *prog = this->useShader(SHADER_solidColor);
	glUniform4f(prog->getShaderLocation(true, SHADER_solidColor_solidColor), color.r, color.g, color.b, color.a);
	BasicShapes::drawLine(position, position+direction, getVertexPosition());
	this->popMatrixM();
}
void RenderManager::renderDirectionVectors(const glm::vec3 position, const glm::vec3 directionForward, const glm::vec3 direction2, const glm::vec4 direction2_color)
{
	this->renderDirectionVector(position, directionForward, glm::vec4(0.f, 1.f, 0.f, 1.f));
	this->renderDirectionVector(position+directionForward, direction2*0.2f, direction2_color);
}
void RenderManager::renderOrientation(const glm::vec3 position, const glm::quat q)
{
	this->renderDirectionVectors(position, q*glm::vec3(0,1,0), q*glm::vec3(0,0,1), glm::vec4(0.f, 0.f, 1.f, 1.f));
}
