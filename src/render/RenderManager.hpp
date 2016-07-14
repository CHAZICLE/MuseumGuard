#ifndef 3YP_RENDER_RENDERMANAGER_HPP_
#define 3YP_RENDER_RENDERMANAGER_HPP_

namespace render {
	namespace shaders {
		class ShaderProgram;
	}
}

#include <glm/matrix.hpp>
#include <util/gl.h>
#include "shaders/ShaderPrograms.h"
#include <glm/gtc/quaternion.hpp>

namespace render {

	class RenderManager {
	private:
		bool mDirty,vDirty,pDirty,mvDirty,vpDirty,mvpDirty;
		bool doCullFace,doDepthBuffer;
		glm::mat4 MV,VP,MVP,stackM,stackMV,stackMVP;
		int widthPx,heightPx;
		float widthMM,heightMM;
		shaders::ShaderProgram *shader;
	public:
		RenderManager();
		virtual ~RenderManager();


		// Matrix
		void setMVPMatrix(GLuint mvpMatrixShaderLocation);

		glm::mat4 M,V,P;

		void markPDirty();
		void markVDirty();
		void markMDirty();

		void pushMatrixM();
		void popMatrixM();

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

		// Debug
		void renderDirectionVector(const glm::vec3 position, const glm::vec3 direction, const glm::vec4 color);
		void renderDirectionVectors(const glm::vec3 position, const glm::vec3 directionForward, const glm::vec3 direction2, const glm::vec4 direction2_color);
		void renderOrientation(const glm::vec3 position, const glm::quat q);
	};

}

#endif
