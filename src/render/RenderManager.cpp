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

	void RenderManager::enableDepth() {
		this->doDepthBuffer = true;
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}
	void RenderManager::cullFace() {
		this->doCullFace = true;
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
	}

}

