#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "BasicShapes.hpp"

GLuint BasicShapes::unitMeshArrayID = 0;
GLuint BasicShapes::unitMeshVertexPositionBufferID = 0;
GLuint BasicShapes::unitSquareIndexBufferID = 0;
GLuint BasicShapes::unitCubeIndexBufferID = 0;
GLuint BasicShapes::lineVertexArrayID = 0;
GLuint BasicShapes::lineVertexBufferID = 0;

void BasicShapes::init()
{
	glGenVertexArrays(1, &BasicShapes::unitMeshArrayID);
	glGenBuffers(1, &BasicShapes::unitMeshVertexPositionBufferID);
	glGenBuffers(1, &BasicShapes::unitSquareIndexBufferID);
	glGenBuffers(1, &BasicShapes::unitCubeIndexBufferID);

	glGenVertexArrays(1, &BasicShapes::lineVertexArrayID);
	glGenBuffers(1, &BasicShapes::lineVertexBufferID);
	
	GLubyte unitMesh[] = {
		// Unit square
		0,	0,	0,
		0,	1,	0,
		1,	1,	0,
		1,	0,	0,
		// Close face of Cube
		0,	0,	0,//4
		0,	1,	0,//5
		1,	1,	0,//6
		1,	0,	0,//7
		// Far face of Cube
		0,	0,	1,//8
		0,	1,	1,//9
		1,	1,	1,//10
		1,	0,	1,//11
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
	
	GLubyte unitCubeIndicies[] = {
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
