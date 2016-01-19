#ifndef __BASICSHAPES_H_INCLUDED__
#define __BASICSHAPES_H_INCLUDED__

#include <glm/glm.hpp>
#include "util/gl.h"

class BasicShapes {
	private:
		static GLuint unitMeshArrayID,
			      unitMeshVertexPositionBufferID,
			      unitSquareIndexBufferID,
			      lineVertexArrayID,
			      lineVertexBufferID
			;
		static void bindUnitMesh(GLuint vertexPositionPointer);
	public:
		static void init();
		static void renderUnitSquare(GLuint vertexPositionPointer);
		static void drawLine(glm::vec3 start, glm::vec3 end, GLuint vertexPositionPointer);
		static void drawPoint(float size, GLuint vertexPositionPointer);
};

#endif
