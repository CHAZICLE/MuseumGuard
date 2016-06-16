#ifndef __RENDERMANAGER_H_INCLUDED__
#define __RENDERMANAGER_H_INCLUDED__

namespace render {
	namespace shaders {
		class ShaderProgram;
	}
}

#include <glm/matrix.hpp>
#include <util/gl.h>
#include "shaders/ShaderPrograms.h"

namespace render {

	class RenderManager {
	private:
		bool mDirty,vDirty,pDirty,mvDirty,vpDirty,mvpDirty;
		bool doCullFace,doDepthBuffer;
		glm::mat4 MV,VP,MVP;
		int widthPx,heightPx;
		float widthMM,heightMM;
		shaders::ShaderProgram *shader;
	public:
		RenderManager();
		virtual ~RenderManager();
		void setMVPMatrix(GLuint mvpMatrixShaderLocation);

		glm::mat4 M,V,P;

		void markPDirty();
		void markVDirty();
		void markMDirty();

		void setShaderMatricies(shaders::ShaderProgram &shaderProgram);
		shaders::ShaderProgram *useShader(int shader);
		GLint getVertexPosition();
		GLint getVertexNormal();
		GLint getVertexTexture();

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
