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
	void RenderManager::disableDepth() {
		glDisable(GL_DEPTH_TEST);
	}
	void RenderManager::enableCullFace() {
		this->doCullFace = true;
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
	void RenderManager::disableCullFace() {
		this->doCullFace = false;
		glDisable(GL_CULL_FACE);
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

}

