#ifndef __RENDERMANAGER_H_INCLUDED__
#define __RENDERMANAGER_H_INCLUDED__

#include <glm/matrix.hpp>
#include <util/gl.h>

namespace render {

	class RenderManager {
	private:
		bool mDirty,vDirty,pDirty,mvDirty,vpDirty,mvpDirty;
		bool doCullFace,doDepthBuffer;
		glm::mat4 MV,MP,MVP;
		int widthPx,heightPx;
		float widthMM,heightMM;
	public:
		RenderManager();
		virtual ~RenderManager();

		glm::mat4 M,V,P;

		void markPDirty();
		void markVDirty();
		void markMDirty();

		void setMVPMatrix(GLuint mvpMatrixShaderLocation);

		void enableDepth();
		void disableDepth();
		void enableCullFace();
		void disableCullFace();
		void enableTransparency();
		void disableTransparency();

		void setDimensionsPx(int widthPx, int heightPx);
		void setDimensionsMM(float widthMM, float heightMM);
		int getWidthPx();
		int getHeightPx();
		float getWidthMM();
		float getHeightMM();
	};

}

#endif
