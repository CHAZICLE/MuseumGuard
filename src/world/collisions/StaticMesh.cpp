#include "tribox3.h"
#include "render/StaticModel.hpp"

//debug
#include "render/shaders/ShaderProgram.hpp"
#include <iostream>

#include "StaticMesh.hpp"

using namespace render;
using namespace world::collisions;
using namespace util::Boundaries;

#define CELL_MAX_LEVEL 12
#define CELL_MAX_TRIANGLES_PER_CELL 16

StaticMesh::StaticMesh()
{
	
}
void deleteCell(StaticMeshCell &cell)
{
	for(auto &c : cell.children)
		deleteCell(c);
	delete cell.bounds;
}
StaticMesh::~StaticMesh()
{
	// Delete all the bounding boxes
	for(auto &c : this->rootCell.children)
		deleteCell(c);
}
// Adding a static model to the static mesh collision
void StaticMesh::addStaticModel(render::StaticModel *model)
{
	int objectCount = 0;
	StaticMeshTriangle triangle;
	triangle.model = model;
	this->rootCell.bounds = &model->getBounds();
	this->rootCell.level = 0;
	this->rootCell.leaf = true;
	this->rootCell.children.clear();
	this->rootCell.triangles.clear();
	for(render::StaticModelObject *oj : model->objects)
	{
		triangle.modelObject = oj;
		for(int i=0;i<oj->numPrimitives;i++)
		{
			//std::cout << "Begin Triangle" << std::endl;
			triangle.triangleIndex = i;
			addTriangleToCell(triangle, this->rootCell, 0);
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
	this->renderCellChildren(rManager, selectionBox, this->rootCell);
}
void StaticMesh::renderCellChildren(render::RenderManager &rManager, AABB &selectionBox, StaticMeshCell &cell)
{
	if(!selectionBox.checkIntersect(*cell.bounds))
		return;
	cell.bounds->render(rManager, glm::vec4(0.f, 1.f, (float)cell.level/CELL_MAX_LEVEL, 1.0f), false);
	for(StaticMeshCell &c : cell.children)
	{
		this->renderCellChildren(rManager, selectionBox, c);
	}
}
void StaticMesh::render(render::RenderManager &rManager, Raycast &raycast)
{
	RaycastResult result;
	result.distance = raycast.maxDistance<=0 ? std::numeric_limits<float>::max() : raycast.maxDistance;
	StaticMeshTriangle *triangle = this->renderCellChildren(rManager, raycast, this->rootCell, result);
	if(triangle!=0)
	{
		glm::vec3 &V0 = *(glm::vec3 *)&triangle->model->dataBuffer[triangle->model->dataBufferStride*triangle->modelObject->indecies[triangle->triangleIndex*3+0]];
		glm::vec3 &V1 = *(glm::vec3 *)&triangle->model->dataBuffer[triangle->model->dataBufferStride*triangle->modelObject->indecies[triangle->triangleIndex*3+1]];
		glm::vec3 &V2 = *(glm::vec3 *)&triangle->model->dataBuffer[triangle->model->dataBufferStride*triangle->modelObject->indecies[triangle->triangleIndex*3+2]];

		render::shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
		glUniform4f(shader->getShaderLocation(true, SHADER_solidColor_solidColor), 0.7f, 0.7f, 0.7f, 0.7f);

		BasicShapes::drawLine(V0, V1, shader->getShaderLocation(false, SHADERVAR_vertex_position));
		BasicShapes::drawLine(V1, V2, shader->getShaderLocation(false, SHADERVAR_vertex_position));
		BasicShapes::drawLine(V0, V2, shader->getShaderLocation(false, SHADERVAR_vertex_position));
	}
}
StaticMeshTriangle *StaticMesh::renderCellChildren(render::RenderManager &rManager, Raycast &raycast, StaticMeshCell &cell, RaycastResult &result)
{
	cell.bounds->render(rManager, glm::vec4(0.f, 1.f, (float)cell.level/CELL_MAX_LEVEL, 1.0f), false);
	StaticMeshTriangle *resultTriangle = 0,*triangleTmp=0;
	float cellDistance = 0;
	for(StaticMeshCell &cell : cell.children)
	{
		cellDistance = cell.bounds->rayCastDistance(raycast);
		if(cell.bounds->checkInside(raycast.origin) || (cellDistance!=-1 && cellDistance<result.distance))
		{
			if(cell.leaf)
				triangleTmp = this->rayCastCellTriangles(raycast, cell, result);
			else
				triangleTmp = this->renderCellChildren(rManager, raycast, cell, result);
			if(triangleTmp!=0)
				resultTriangle = triangleTmp;
		}
	}
	return resultTriangle;
}


//##Raycasting functions
RaycastResult StaticMesh::rayCast(Raycast &raycast)
{
	RaycastResult result;
	result.distance = raycast.maxDistance<=0 ? std::numeric_limits<float>::max() : raycast.maxDistance;
	this->rayCastCellChildren(raycast, this->rootCell, result);
	result.hit = result.distance>=0 && result.distance<raycast.maxDistance;
	return result;
}
StaticMeshTriangle *StaticMesh::rayCastCellChildren(Raycast &raycast, StaticMeshCell &cell, RaycastResult &result)
{
	StaticMeshTriangle *resultTriangle = 0,*triangleTmp=0;
	float cellDistance = 0;
	for(StaticMeshCell &cell : cell.children)
	{
		cellDistance = cell.bounds->rayCastDistance(raycast);
		if(cell.bounds->checkInside(raycast.origin) || (cellDistance!=-1 && cellDistance<result.distance))
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
	for(StaticMeshTriangle &triangle : cell.triangles)
	{
		glm::vec3 &P0 = raycast.origin;
		glm::vec3 &P1 = raycast.direction;
		glm::vec3 &V0 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]];
		glm::vec3 &V1 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]];
		glm::vec3 &V2 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]];
		glm::vec3 u = V1-V0;
		glm::vec3 v = V2-V0;
		glm::vec3 n = glm::cross(u,v);
		// Determine where the ray is hitting the plane
		float rI_d = glm::dot(n, P1);
		if(rI_d==0)//Ray is parallel
			continue;
		float rI = glm::dot(n, V0-P0)/rI_d;
		if(result.distance<rI)//Ray is further away
			continue;
		// Is the projected point on the plane within the triangle?
		glm::vec3 w = (P0+P1*rI)-V0;
		float dotUV = glm::dot(u,v);
		float dotWU = glm::dot(w,u);
		float dotWV = glm::dot(w,v);
		float dotVV = glm::dot(v,v);
		float dotUU = glm::dot(u,u);
		float _I_d = dotUV*dotUV-dotUU*dotVV;
		float sI = (dotUV*dotWV-dotVV*dotWU)/_I_d;
		float tI = (dotUV*dotWU-dotUU*dotWV)/_I_d;
		if(sI>=0 && tI>=0 && sI+tI<=1)
		{
			//std::cout << "SUCC rI=" << rI << "sI=" << sI << ", tI=" << tI << ", sI+tI=" << (sI+tI) << std::endl;
			if(rI>0 && result.distance>rI)
			{
				result.distance = rI;
				glm::vec3 &N0 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]+triangle.model->dataBufferNormalsOffset];
				glm::vec3 &N1 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]+triangle.model->dataBufferNormalsOffset];
				glm::vec3 &N2 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]+triangle.model->dataBufferNormalsOffset];
				result.hitNormal = N0*(1-(sI+tI))+N1*sI+N2*tI;
				//result.hitNormal = n;
				//std::cout << "Total:" << (1-(sI+tI))+sI+tI << std::endl;
				//if(glm::dot(result.hitNormal, P1)>0)//Ensure facing towards the ray origin
				//	result.hitNormal = -result.hitNormal;
			}
		}
	}
	return resultTriangle;
}
glm::vec3 StaticMesh::getTriangleNormal(const StaticMeshTriangle &triangle)
{
	glm::vec3 &N0 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]+triangle.model->dataBufferNormalsOffset];
	glm::vec3 &N1 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]+triangle.model->dataBufferNormalsOffset];
	glm::vec3 &N2 = *(glm::vec3 *)&triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]+triangle.model->dataBufferNormalsOffset];
	return glm::normalize((N0+N1+N2));
}
bool StaticMesh::checkIntersect(render::RenderManager &rManager, const AABB &aabb, glm::vec3 *velocity)
{
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	if(!this->rootCell.bounds->checkIntersect(aabb))
		return false;
	//rootCell.bounds->render(rManager, glm::vec4(0.f, 1.f, (float)rootCell.level/CELL_MAX_LEVEL, 1.0f), false);
	return this->checkIntersectCellChildren(rManager, aabb, this->rootCell, velocity);
}
bool StaticMesh::checkIntersectCellChildren(render::RenderManager &rManager, const AABB &aabb, const StaticMeshCell &parent, glm::vec3 *velocity)
{
	//parent.bounds->render(rManager, glm::vec4(0.f, 1.f, (float)parent.level/CELL_MAX_LEVEL, 1.0f), false);
	bool intersection = false;
	for(const StaticMeshCell &cell : parent.children)
	{
		if(cell.leaf)
		{
			for(const StaticMeshTriangle &triangle : cell.triangles)
			{
				GLfloat *vertexAPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+0]];
				GLfloat *vertexBPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+1]];
				GLfloat *vertexCPosition = &triangle.model->dataBuffer[triangle.model->dataBufferStride*triangle.modelObject->indecies[triangle.triangleIndex*3+2]];
				AABB aabb2 = aabb;
				if(triBoxOverlap(aabb2.boxCenter, aabb2.boxHalfSize, vertexAPosition, vertexBPosition, vertexCPosition))
				{
					//std::cout << "TRIANGLE INTERSECT" << std::endl;
					if(velocity==0)
					{
						aabb2.render(rManager, glm::vec4(0.f, 1.f, 1.f, 1.0f), false);
						return true;
					}
					else
					{
						glm::vec3 normal = getTriangleNormal(triangle);
						if(glm::dot(normal, *velocity)<0)
						{
							*velocity -= (normal*glm::dot(*velocity, normal));
							intersection = true;
						}
					}
				}
			}
		}
		else
		{
			if(cell.bounds->checkIntersect(aabb) && this->checkIntersectCellChildren(rManager, aabb, cell, velocity))
			{
				//if(velocity==0)
				//	return true;//Don't need to bother to check everything
				intersection = true;
			}
		}
	}
	return intersection;
}
bool StaticMesh::checkIntersect(const util::Boundaries::RBB &rbb)
{
	return false;
}
