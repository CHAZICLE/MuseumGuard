#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Boundaries.hpp"

using namespace render;
using namespace shaders;
using namespace util;
using namespace Boundaries;

OBB::OBB(const glm::vec3 &min, const glm::vec3 &max)
{
	this->min = min;
	this->max = max;
}
OBB::OBB(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ)
{
	this->min.x = minX;
	this->min.y = minY;
	this->min.z = minZ;
	this->max.x = maxX;
	this->max.y = maxY;
	this->max.z = maxZ;
}

AABB::AABB()
{
	AABB(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
}
AABB::AABB(const float boxCenterX, const float boxCenterY, const float boxCenterZ, const float boxHalfSizeX, const float boxHalfSizeY, const float boxHalfSizeZ)
{
	this->boxCenter[0] = boxCenterX;
	this->boxCenter[1] = boxCenterY;
	this->boxCenter[2] = boxCenterZ;
	this->boxHalfSize[0] = boxHalfSizeX;
	this->boxHalfSize[1] = boxHalfSizeY;
	this->boxHalfSize[2] = boxHalfSizeZ;
}
AABB::AABB(const glm::vec3 &min, const glm::vec3 &max)
{
	this->boxCenter[0] = (min.x+max.x)/2.f;
	this->boxCenter[1] = (min.y+max.y)/2.f;
	this->boxCenter[2] = (min.z+max.z)/2.f;
	this->boxHalfSize[0] = (max.x-min.x)/2.f;
	this->boxHalfSize[1] = (max.y-min.y)/2.f;
	this->boxHalfSize[2] = (max.z-min.z)/2.f;
}
AABB::~AABB()
{
	
}
AABB AABB::translate(const glm::vec3 &offset)
{
	AABB aabb(this->min()+offset, this->max()+offset);
	return aabb;
}
OBB AABB::rotate(const glm::quat &rotation)
{
	OBB obb(rotation*this->min(), rotation*this->max());
	return obb;
}
bool AABB::checkInside(const glm::vec3 v)
{
	return
		this->minX()<=v.x && this->maxX()>=v.x &&
		this->minY()<=v.y && this->maxY()>=v.y &&
		this->minZ()<=v.z && this->maxZ()>=v.z
	;
}
bool AABB::checkIntersect(const AABB &aabb)
{
	// a.Min>b.Max || a.Max<b.Min ||
	return !(
		this->boxCenter[0]-this->boxHalfSize[0]>aabb.boxCenter[0]+aabb.boxHalfSize[0] || this->boxCenter[0]+this->boxHalfSize[0]<aabb.boxCenter[0]-aabb.boxHalfSize[0] ||
		this->boxCenter[1]-this->boxHalfSize[1]>aabb.boxCenter[1]+aabb.boxHalfSize[1] || this->boxCenter[1]+this->boxHalfSize[1]<aabb.boxCenter[1]-aabb.boxHalfSize[1] ||
		this->boxCenter[2]-this->boxHalfSize[2]>aabb.boxCenter[2]+aabb.boxHalfSize[2] || this->boxCenter[2]+this->boxHalfSize[2]<aabb.boxCenter[2]-aabb.boxHalfSize[2]
	);
}
bool AABB::checkIntersect(const Sphere &rbb)
{
	return glm::length2(glm::max(glm::min(this->max(), rbb.center), this->min())-rbb.center) < rbb.radius*rbb.radius;
}
void AABB::render(RenderManager &rManager, glm::vec4 color, bool solid)
{
	rManager.pushMatrixM();
	rManager.M = glm::scale(glm::translate(rManager.M, glm::vec3(this->boxCenter[0]-this->boxHalfSize[0], this->boxCenter[1]-this->boxHalfSize[1], this->boxCenter[2]-this->boxHalfSize[2])), glm::vec3(this->boxHalfSize[0]*2,this->boxHalfSize[1]*2,this->boxHalfSize[2]*2));
	rManager.markMDirty();
	if(color.a!=1.0f)
		rManager.enableTransparency();
	ShaderProgram *sp = rManager.useShader(SHADER_solidColor);
	glUniform4f(sp->getShaderLocation(true, SHADER_solidColor_solidColor), color.x, color.y, color.z, color.a);
	if(solid)
		BasicShapes::renderUnitCube(sp->getShaderLocation(false, SHADERVAR_vertex_position));
	else
		BasicShapes::renderUnitCubeFrame(sp->getShaderLocation(false, SHADERVAR_vertex_position));
	if(color.a!=1.0f)
		rManager.disableTransparency();
	rManager.popMatrixM();
}
float AABB::rayCastDistance(Raycast &raycast)
{
	float md = raycast.maxDistance==0 ? std::numeric_limits<float>::max()-1 : raycast.maxDistance;
	float t[2][3];
	t[0][0] = (this->minX()-raycast.origin.x)/raycast.direction.x;
	t[0][1] = (this->minY()-raycast.origin.y)/raycast.direction.y;
	t[0][2] = (this->minZ()-raycast.origin.z)/raycast.direction.z;
	
	t[1][0] = (this->maxX()-raycast.origin.x)/raycast.direction.x;
	t[1][1] = (this->maxY()-raycast.origin.y)/raycast.direction.y;
	t[1][2] = (this->maxZ()-raycast.origin.z)/raycast.direction.z;
	
	float tmax = std::numeric_limits<float>::min();
	float tmin = std::numeric_limits<float>::max();
	float tmp;
	for(int i=0;i<3;i++)
	{
		float &ntMin = t[0][i];
		float &ntMax = t[1][i];
		if(ntMin>ntMax)
		{
			tmp = ntMin;
			ntMin = ntMax;
			ntMax = tmp;
		}
		if(ntMax < tmax || ntMin > tmin)
			return -1;
		if(ntMin > tmax)
			tmax = ntMin;
		if(ntMax < tmin)
			tmin = ntMax;
	}
	if(tmax > tmin || tmax >= md)
		return -1;
	return tmax;
}
bool AABB::rayCastCheck(Raycast &raycast)
{
	return this->rayCastDistance(raycast)!=-1;
}
RaycastResult *AABB::rayCast(Raycast &raycast)
{
	float md = raycast.maxDistance==0 ? std::numeric_limits<float>::max() : raycast.maxDistance;
	float t[2][3];
	t[0][0] = (this->minX()-raycast.origin.x)/raycast.direction.x;
	t[0][1] = (this->minY()-raycast.origin.y)/raycast.direction.y;
	t[0][2] = (this->minZ()-raycast.origin.z)/raycast.direction.z;
	
	t[1][0] = (this->maxX()-raycast.origin.x)/raycast.direction.x;
	t[1][1] = (this->maxY()-raycast.origin.y)/raycast.direction.y;
	t[1][2] = (this->maxZ()-raycast.origin.z)/raycast.direction.z;
	
	float tmax = std::numeric_limits<float>::min();
	float tmin = std::numeric_limits<float>::max();
	float tmp;
	for(int i=0;i<3;i++)
	{
		float &ntMin = t[0][i];
		float &ntMax = t[1][i];
		if(ntMin>ntMax)
		{
			tmp = ntMin;
			ntMin = ntMax;
			ntMax = tmp;
		}
		if(ntMax < tmax || ntMin > tmin)
			return 0;
		if(ntMin > tmax)
			tmax = ntMin;
		if(ntMax < tmin)
			tmin = ntMax;
	}
	if(tmax > tmin || tmax >= md)
		return 0;
	RaycastResult *result = new RaycastResult;
	result->distance = tmax;
	result->hit = true;
	//result->rayHit = glm::normalize(raycast.direction)*tmax;
	return result;
}
glm::vec3 AABB::min()
{
	return glm::vec3(this->minX(), this->minY(), this->minZ());
}
glm::vec3 AABB::max()
{
	return glm::vec3(this->maxX(), this->maxY(), this->maxZ());
}
float AABB::minX() { return this->boxCenter[0]-this->boxHalfSize[0]; }
float AABB::minY() { return this->boxCenter[1]-this->boxHalfSize[1]; }
float AABB::minZ() { return this->boxCenter[2]-this->boxHalfSize[2]; }
float AABB::maxX() { return this->boxCenter[0]+this->boxHalfSize[0]; }
float AABB::maxY() { return this->boxCenter[1]+this->boxHalfSize[1]; }
float AABB::maxZ() { return this->boxCenter[2]+this->boxHalfSize[2]; }
