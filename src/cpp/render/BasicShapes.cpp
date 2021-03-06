#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "BasicShapes.hpp"

using namespace render;

GLuint BasicShapes::unitMeshArrayID = 0;
GLuint BasicShapes::unitMeshVertexPositionBufferID = 0;
GLuint BasicShapes::unitSquareIndexBufferID = 0;
GLuint BasicShapes::unitCubeIndexBufferID = 0;
GLuint BasicShapes::unitCubeFrameIndexBufferID = 0;
GLuint BasicShapes::lineVertexArrayID = 0;
GLuint BasicShapes::lineVertexBufferID = 0;

void BasicShapes::init()
{
	glGenVertexArrays(1, &BasicShapes::unitMeshArrayID);
	glGenBuffers(1, &BasicShapes::unitMeshVertexPositionBufferID);
	glGenBuffers(1, &BasicShapes::unitSquareIndexBufferID);
	glGenBuffers(1, &BasicShapes::unitCubeIndexBufferID);
	glGenBuffers(1, &BasicShapes::unitCubeFrameIndexBufferID);

	glGenVertexArrays(1, &BasicShapes::lineVertexArrayID);
	glGenBuffers(1, &BasicShapes::lineVertexBufferID);
	
	GLubyte unitMesh[] = {
		// Unit square/Close face of Cube
		0,	0,	0,// 0
		0,	1,	0,// 1
		1,	1,	0,// 2
		1,	0,	0,// 3
		// Far face of Cube
		0,	0,	1,// 4
		0,	1,	1,// 5
		1,	1,	1,// 6
		1,	0,	1,// 7
		/*
		 * 9  10
		 * 8  11
		 * |  |
		 * 5  6
		 * 4  7
		 *
		 *
		 */
		
	};
	
	glBindVertexArray(BasicShapes::unitMeshArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, BasicShapes::unitMeshVertexPositionBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitMesh), unitMesh, GL_STATIC_DRAW);
	
	GLubyte unitSquareIndicies[] = {
		0,1,2,
		0,2,3
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BasicShapes::unitSquareIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unitSquareIndicies), unitSquareIndicies, GL_STATIC_DRAW);

	GLubyte unitCubeFrameIndecies[] = {
		// Front
		0,1,
		0,3,
		2,1,
		2,3,
		// Back
		4,5,
		4,7,
		6,5,
		6,7,
		// Depth
		0,4,
		1,5,
		2,6,
		3,7
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BasicShapes::unitCubeFrameIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unitCubeFrameIndecies), unitCubeFrameIndecies, GL_STATIC_DRAW);
	
	GLubyte unitCubeIndicies[] = {
		// Back
		7,6,5,
		7,5,4,
		// Right side
		3,2,6,
		3,6,7,
		// top
		1,5,6,
		1,6,2,
		// Closest face
		0,1,2,
		0,2,3,
		// Left side
		4,5,1,
		4,1,0,
		// bottom
		4,0,3,
		4,3,7,
/*
 *  0
 *  1
 *  2
 *  3
 *  4 0
 *  5 1
 *  6 2
 *  7 3
 *  8 4
 *  9 5
 * 10 6
 * 11 7
 * /


		// Back
		11,10,9,
		11,9,8,
		// Right side
		7,6,10,
		7,10,11,
		// top
		5,9,10,
		5,10,6,
		// Closest face
		4,5,6,
		4,6,7,
		// Left side
		8,9,5,
		8,5,4,
		// bottom
		8,4,7,
		8,7,11,
		*/
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BasicShapes::unitCubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unitCubeIndicies), unitCubeIndicies, GL_STATIC_DRAW);
}
void BasicShapes::renderUnitSquare(GLuint vertexPositionPointer)
{
	glEnableVertexAttribArray(vertexPositionPointer);
	bindUnitMesh(vertexPositionPointer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unitSquareIndexBufferID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void  *)0);
}
void BasicShapes::renderUnitCube(GLuint vertexPositionPointer)
{
	glEnableVertexAttribArray(vertexPositionPointer);
	bindUnitMesh(vertexPositionPointer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unitCubeIndexBufferID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void  *)0);
}
void BasicShapes::renderUnitCubeFrame(GLuint vertexPositionPointer)
{
	glEnableVertexAttribArray(vertexPositionPointer);
	bindUnitMesh(vertexPositionPointer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unitCubeFrameIndexBufferID);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, (void  *)0);
}
void BasicShapes::bindUnitMesh(GLuint vertexPositionPointer)
{
	glBindVertexArray(unitMeshArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, unitMeshVertexPositionBufferID);
	glVertexAttribPointer(vertexPositionPointer, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
}
void BasicShapes::drawLine(glm::vec3 start, glm::vec3 end, GLuint vertexPositionPointer)
{
	GLfloat data[] = {
		start.x, start.y, start.z,
		end.x, end.y, end.z
	};
	glBindVertexArray(BasicShapes::lineVertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, BasicShapes::lineVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(vertexPositionPointer);
	glVertexAttribPointer(vertexPositionPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, 2);
}
void BasicShapes::drawPoint(float size, GLuint vertexPositionPointer)
{
	glPointSize(size);
	bindUnitMesh(vertexPositionPointer);
	glDrawArrays(GL_POINTS, 0, 1);
}
