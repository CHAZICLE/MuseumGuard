/*
 * RenderManager.cpp
 *
 *  Created on: 31 Jan 2016
 *      Author: charles
 */

#include "RenderManager.hpp"
#include "util/gl.h"

namespace render {

RenderManager::RenderManager() {
	this->mDirty = false;
	this->vDirty = false;
	this->pDirty = false;
	this->mvDirty = false;
	this->vpDirty = false;
	this->mvpDirty = false;
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


} /* namespace render */

