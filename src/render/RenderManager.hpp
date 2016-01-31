/*
 * RenderManager.hpp
 *
 *  Created on: 31 Jan 2016
 *      Author: charles
 */

#ifndef SRC_RENDER_RENDERMANAGER_HPP_
#define SRC_RENDER_RENDERMANAGER_HPP_

#include <glm/matrix.hpp>
#include <util/gl.h>

namespace render {

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager();

	glm::mat4 MVP;

	void setMVPMatrix(GLuint mvpMatrixShaderLocation);
};

} /* namespace render */

#endif /* SRC_RENDER_RENDERMANAGER_HPP_ */
