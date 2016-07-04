#include "tribox2.h"
#include "render/StaticModel.hpp"

//debug
#include "render/shaders/ShaderProgram.hpp"
#include <iostream>

#include "StaticMesh.hpp"

using namespace render;
using namespace world::collisions;
using namespace util::Boundaries;

#define CELL_MAX_LEVEL 8
#define CELL_MAX_TRIANGLES_PER_CELL 1

StaticMesh::StaticMesh()
{
	
}
StaticMesh::~StaticMesh()
{
	
}
// Adding a static model to the static mesh collision
void StaticMesh::addStaticModel(render::StaticModel *model)
{
	int objectCount = 0;
	StaticMeshTriangle triangle;
	triangle.model = model;
	this->rootNode.bounds = &model->getBounds();
	this->rootNode.level = 0;
	this->rootNode.leaf = true;
	this->rootNode.children.clear();
	this->rootNode.triangles.clear();
	for(render::StaticModelObject *oj : model->objects)
	{
		triangle.modelObject = oj;
		for(int i=0;i<oj->numPrimitives;i++)
		{
			//std::cout << "Begin Triangle" << std::endl;
			triangle.triangleIndex = i;
			addTriangleToCell(triangle, this->rootNode, 0);
		}
		objectCount++;
	}
}
void StaticMesh::addTriangleToCell(StaticMeshTriangle &triangle, StaticMeshCell &cell, int cellLevel)
{
	if(cell.leaf)
	{
		cell.triangles.push_back(triangle);
		if(cell.triangles.size()>CELL_MAX_TRIANGLES_PER_CELL && cell.level<=CELL_MAX_LEVEL)
		{
			//std::cout << "\t" << cellLevel << ":S cellTriangleCount=" << cellTriangleCount << std::endl;
			// Add triangles from this ex-leaf cell to child cells
			cell.leaf = false;
			subdivideCell(cell, cellLevel);
			for(auto tri : cell.triangles)
			{
				addTriangleToCellChildren(tri, cell, cellLevel+1);
			}
			// Non leaf nodes don't need triangles in them
			cell.triangles.clear();
		}
	}
	else
	{
		//std::cout << "\t" << cellLevel << ":N cellTriangleCount=" << cellTriangleCount << std::endl;
		addTriangleToCellChildren(triangle, cell, cellLevel+1);
	}
	//std::cout << "\t" << cellLevel << ":T cellTriangleCount=" << cell.triangles.size() << std::endl;
}
void StaticMesh::addTriangleToCellChildren(StaticMeshTriangle &triangle, StaticMeshCell &parent, int childrenLevel)
{
	for(StaticMeshCell &child : parent.children)
	{
		// Get the vertecies of the triangle
		GLfloat *vertexAPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]];
		GLfloat *vertexBPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]];
		GLfloat *vertexCPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]];
		if(triBoxOverlap(child.bounds->boxCenter, child.bounds->boxHalfSize, vertexAPosition, vertexBPosition, vertexCPosition))
		{
			this->addTriangleToCell(triangle, child, childrenLevel);
		}
	}
}
// Cell subdivision functions
void StaticMesh::subdivideCell(StaticMeshCell &parent, int childrenLevel)
{
	//std::cout << "subdivide: Level:" << childrenLevel << std::endl;
#define X 0
#define Y 1
#define Z 2
	// Determine which should be split
	bool splitX = parent.bounds->boxHalfSize[X]*2>parent.bounds->boxHalfSize[Y] || parent.bounds->boxHalfSize[X]*2>parent.bounds->boxHalfSize[Z];
	bool splitY = parent.bounds->boxHalfSize[Y]*2>parent.bounds->boxHalfSize[X] || parent.bounds->boxHalfSize[Y]*2>parent.bounds->boxHalfSize[Z];
	bool splitZ = parent.bounds->boxHalfSize[Z]*2>parent.bounds->boxHalfSize[X] || parent.bounds->boxHalfSize[Z]*2>parent.bounds->boxHalfSize[Y];
	
	// Calculate the sizes of the new cells
	float boxSplitHalfSize[] = {
		splitX ? parent.bounds->boxHalfSize[X]/2 : parent.bounds->boxHalfSize[X],
		splitY ? parent.bounds->boxHalfSize[Y]/2 : parent.bounds->boxHalfSize[Y],
		splitZ ? parent.bounds->boxHalfSize[Z]/2 : parent.bounds->boxHalfSize[Z]
	};
	// Calculate the centers of all the lower set of cells
	float boxSplitCenter1[] = {
		splitX ? parent.bounds->boxCenter[X]-parent.bounds->boxHalfSize[X]/2: parent.bounds->boxCenter[X],
		splitY ? parent.bounds->boxCenter[Y]-parent.bounds->boxHalfSize[Y]/2: parent.bounds->boxCenter[Y],
		splitZ ? parent.bounds->boxCenter[Z]-parent.bounds->boxHalfSize[Z]/2: parent.bounds->boxCenter[Z],
	};
	// Calculate the centers of all the upper set of cells
	float boxSplitCenter2[] = {
		boxSplitCenter1[X]+parent.bounds->boxHalfSize[X],
		boxSplitCenter1[Y]+parent.bounds->boxHalfSize[Y],
		boxSplitCenter1[Z]+parent.bounds->boxHalfSize[Z]
	};
	
						addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter1[X], boxSplitCenter1[Y], boxSplitCenter1[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitZ)				addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter1[X], boxSplitCenter1[Y], boxSplitCenter2[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitY)				addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter1[X], boxSplitCenter2[Y], boxSplitCenter1[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitY && splitZ)			addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter1[X], boxSplitCenter2[Y], boxSplitCenter2[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitX)				addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter2[X], boxSplitCenter1[Y], boxSplitCenter1[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitX && splitZ)			addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter2[X], boxSplitCenter1[Y], boxSplitCenter2[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitX && splitY)			addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter2[X], boxSplitCenter2[Y], boxSplitCenter1[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
	if(splitX && splitY && splitZ)		addChildToCell(parent,  childrenLevel,  new AABB(boxSplitCenter2[X], boxSplitCenter2[Y], boxSplitCenter2[Z], boxSplitHalfSize[X], boxSplitHalfSize[Y], boxSplitHalfSize[Z]));
}
void StaticMesh::addChildToCell(StaticMeshCell &parent, int childLevel, AABB *boxBounds)
{
	StaticMeshCell cell;
	cell.bounds = boxBounds;
	cell.level = childLevel;
	cell.leaf = true;
	cell.children.clear();
	cell.triangles.clear();
	parent.children.push_back(cell);
}


//## Debug rendering functions
void StaticMesh::render(render::RenderManager &rManager, AABB &selectionBox)
{
	//render::shaders::ShaderProgram *proc = rManager.useShader(SHADER_solidColor);
	//render::BasicShapes::renderUnitCube(proc->getShaderLocation(false, SHADERVAR_vertex_position));
	//this->rootNode.bounds->render(rManager, glm::vec4(0.f, 1.f, 0.f, 1.0f), false);
	this->renderCellChildren(rManager, selectionBox, &this->rootNode);
}
void StaticMesh::renderCellChildren(render::RenderManager &rManager, AABB &selectionBox, StaticMeshCell *cell)
{
//std::cout  << "RENDE" << std::endl;
	if(!selectionBox.checkIntersect(*cell->bounds))
		return;
	cell->bounds->render(rManager, glm::vec4(0.f, 1.f, (float)cell->level/CELL_MAX_LEVEL, 1.0f), false);
	for(StaticMeshCell &c : cell->children)
	{
		this->renderCellChildren(rManager, selectionBox, &c);
	}
}


//##Raycasting functions
RaycastResult *StaticMesh::rayCast(Raycast &raycast)
{
	RaycastResult *result = new RaycastResult;
	result->distance = std::numeric_limits<float>::max();
	this->rayCastCellChildren(raycast, this->rootNode, *result);
	if(result->distance<0 || result->distance==std::numeric_limits<float>::max())
		return 0;
	return result;
}
StaticMeshTriangle *StaticMesh::rayCastCellChildren(Raycast &raycast, StaticMeshCell &cell, RaycastResult &result)
{
	StaticMeshTriangle *resultTriangle = 0,*triangleTmp=0;
	float cellDistance;
	for(StaticMeshCell &cell : cell.children)
	{
		cellDistance = cell.bounds->rayCastDistance(raycast);
		//if(cellDistance!=-1 && cellDistance<=result.distance)
		if(cellDistance!=-1)
		{
			if(cell.leaf)
				triangleTmp = this->rayCastCellTriangles(raycast, cell, result);
			else
				triangleTmp = this->rayCastCellChildren(raycast, cell, result);
			if(triangleTmp!=0)
				resultTriangle = triangleTmp;
		}
	}
	return resultTriangle;
}
StaticMeshTriangle *StaticMesh::rayCastCellTriangles(Raycast &raycast, StaticMeshCell &cell, RaycastResult &result)
{
	StaticMeshTriangle *resultTriangle = 0;
	glm::vec3 plainDirection[2],pvec,tvec,qvec;
	float det,t,u,v;
	bool invert = false;
	for(StaticMeshTriangle &triangle : cell.triangles)
	{
		GLfloat *vertexAPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]];
		GLfloat *vertexBPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]];
		GLfloat *vertexCPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]];
		plainDirection[0].x = vertexBPosition[0]-vertexAPosition[0];
		plainDirection[0].y = vertexBPosition[1]-vertexAPosition[1];
		plainDirection[0].z = vertexBPosition[2]-vertexAPosition[2];
		plainDirection[1].x = vertexCPosition[0]-vertexAPosition[0];
		plainDirection[1].y = vertexCPosition[1]-vertexAPosition[1];
		plainDirection[1].z = vertexCPosition[2]-vertexAPosition[2];
		pvec = glm::cross(raycast.rayDirection, plainDirection[1]);
		det = glm::dot(plainDirection[0], pvec);
		//result.distance = det;
		if(det>0.00001 && det<0.00001)//If ray is parallel(ish) to the plane
		{
			continue;
		}
		tvec.x = raycast.rayOrigin.x-vertexAPosition[0];
		tvec.y = raycast.rayOrigin.y-vertexAPosition[1];
		tvec.z = raycast.rayOrigin.z-vertexAPosition[2];
		qvec = glm::cross(tvec, plainDirection[0]);
		u = glm::dot(tvec, pvec);
		v = glm::dot(raycast.rayDirection, qvec);
		t = glm::dot(plainDirection[1], qvec)/det;
		if(det<0)
		{
			det = -det;
			u = -u;
			v = -v;
			invert = true;
		}
		if(u<0 || u>det)
		{
			continue;
		}
		if(v<0 || u+v>det)
		{
			continue;
		}
		if(t<result.distance)
		{
			result.distance = t;
			if(invert)
				result.hitNormal = glm::normalize(glm::cross(plainDirection[1], plainDirection[0]));
			else
				result.hitNormal = glm::normalize(glm::cross(plainDirection[0], plainDirection[1]));
			resultTriangle = &triangle;
		}
	}
	return resultTriangle;
}
