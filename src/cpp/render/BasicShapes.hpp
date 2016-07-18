#ifndef FYP_RENDER_BASICSHAPES_HPP_
#define FYP_RENDER_BASICSHAPES_HPP_

#include <glm/glm.hpp>
#include "util/gl.h"

namespace render {
	class BasicShapes {
		private:
			static GLuint unitMeshArrayID,
					unitMeshVertexPositionBufferID,
					unitSquareIndexBufferID,
					unitCubeIndexBufferID,
					lineVertexArrayID,
					lineVertexBufferID,
					unitCubeFrameIndexBufferID
				;
			static void bindUnitMesh(GLuint vertexPositionPointer);
		public:
			static void init();
			static void renderUnitSquare(GLuint vertexPositionPointer);
			static void renderUnitCube(GLuint vertexPositionPointer);
			static void renderUnitCubeFrame(GLuint vertexPositionPointer);
			static void drawLine(glm::vec3 start, glm::vec3 end, GLuint vertexPositionPointer);
			static void drawPoint(float size, GLuint vertexPositionPointer);
	};
}

#endif
