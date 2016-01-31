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
	// TODO Auto-generated constructor stub

}

RenderManager::~RenderManager() {
	// TODO Auto-generated destructor stub
}

void RenderManager::setMVPMatrix(GLuint mvpMatrixShaderLocation) {
	glUniformMatrix4fv(mvpMatrixShaderLocation, 1, GL_FALSE, &MVP[0][0]);
}

} /* namespace render */
