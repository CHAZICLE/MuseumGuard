#include "util/Globals.hpp"
#include <istream>
#include <iostream>
#include <unordered_map>
#include "util/StreamUtils.hpp"
#include "util/gl.h"
#include "render/RenderManager.hpp"
#include "render/shaders/ShaderProgram.hpp"
#include "render/MaterialLibrary.hpp"
#include "util/AssetUtils.hpp"
#include "util/Boundaries.hpp"

#include "StaticModel.hpp"

using namespace render;
using namespace util::StreamUtils;
using namespace util::Boundaries;

StaticModel::StaticModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));
	this->bounds = new AABB();
	
	// int dataBufferStride = 0; - Field
	dataBufferStride = 3;
	dataBufferNormalsOffset = 0;
	dataBufferColorsOffset = 0;
	int vertexStride = 1;
	int vertexTexturesOffset = 0;
	int vertexNormalsOffset = 0;
	float *vertexPositions,*vertexTextures,*vertexNormals,*vertexColors;
	//std::vector<float> dataBuffer;//[[v[0], v[1], v[2], vt[0], vt[1], vn[0], vn[1], vn[2]]...]
	std::unordered_map<struct FaceKey, int, FaceKeyHasher> assocMap;//[v index, vt index, vn index] : [faceIndex]
	
	//vertex positions
	lenVertexPositions = readInt(fp);
	vertexPositions = new float[lenVertexPositions*3];
	fp.read((char *)vertexPositions, sizeof(float)*lenVertexPositions*3);
	//vertex textures
	int lenVertexTextures = readInt(fp);
	if(lenVertexTextures>0)//enableTextures
	{
		vertexTextures = new float[lenVertexTextures*2];
		fp.read((char *)vertexTextures, sizeof(float)*lenVertexTextures*2);
		dataBufferStride += 2;
		vertexStride += 1;
		vertexTexturesOffset = 1;
	}
	//vertex normals
	int lenVertexNormals = readInt(fp);
	if(lenVertexNormals>0)//enableNormals
	{
		vertexNormals = new float[lenVertexNormals*3];
		fp.read((char *)vertexNormals, sizeof(float)*lenVertexNormals*3);
		dataBufferNormalsOffset = dataBufferStride;
		dataBufferStride += 3;
		vertexStride += 1;
		vertexNormalsOffset = vertexTexturesOffset+1;
	}
	//vertex colors
	int lenVertexColors = readInt(fp);
	if(lenVertexColors>0)//enableNormals
	{
		vertexColors = new float[lenVertexColors*3];
		fp.read((char *)vertexColors, sizeof(float)*lenVertexColors*3);
		dataBufferColorsOffset = dataBufferStride;
		dataBufferStride += 3;
		vertexStride += 1;
		vertexNormalsOffset = vertexTexturesOffset+1;
	}
	//objects
	int lenObjects = readInt(fp);
	struct FaceKey faceKey;
	int currentVertexIndex = 0;
	temp_totalVertexCount = 0;
	float minX = NAN;
	float minY = NAN;
	float minZ = NAN;
	float maxX = NAN;
	float maxY = NAN;
	float maxZ = NAN;
	for(int i=0;i<lenObjects;i++)
	{
		StaticModelObject *o = new StaticModelObject;
		// Load the wavefront object
		o->name = readString(fp);
		o->mtlAsset = readInt(fp);
		o->materialId = readInt(fp);
		o->s = readBool(fp);
		o->numPrimitives = readInt(fp);
		int numVerticies = o->numPrimitives*3;
		int *objectIndecies = new int[numVerticies*vertexStride];
		o->indecies = new GLuint[numVerticies];
		fp.read((char *) objectIndecies, sizeof(int)*numVerticies*vertexStride);
		// Store the faces
		temp_totalVertexCount+=numVerticies;
		for(int v=0;v<numVerticies;v++)
		{
			// Load the vertex indexes for the components of the vertex
			faceKey.vertexPositionIndex = objectIndecies[v*vertexStride]-1;
			faceKey.vertexTextureIndex = (lenVertexTextures>0 ? objectIndecies[v*vertexStride+vertexTexturesOffset]-1 : -1);
			faceKey.vertexNormalIndex = (lenVertexNormals>0 ? objectIndecies[v*vertexStride+vertexNormalsOffset]-1 : -1);
			
			std::unordered_map<struct FaceKey, int, FaceKeyHasher>::const_iterator findFace = assocMap.find(faceKey);
			if(findFace==assocMap.end())//If vertex is not already associated
			{
				// Associate the store the new index
				assocMap[faceKey] = currentVertexIndex;
				o->indecies[v] = currentVertexIndex;
				currentVertexIndex++;
				// Get the vertex data
				float &vX = vertexPositions[faceKey.vertexPositionIndex*3+0];
				float &vY = vertexPositions[faceKey.vertexPositionIndex*3+1];
				float &vZ = vertexPositions[faceKey.vertexPositionIndex*3+2];
				// Update the min/max
				if(minX!=minX || vX<minX) minX = vX-0.001f;
				if(minY!=minY || vY<minY) minY = vY-0.001f;
				if(minZ!=minZ || vZ<minZ) minZ = vZ-0.001f;
				if(maxX!=maxX || vX>maxX) maxX = vX+0.001f;
				if(maxY!=maxY || vY>maxY) maxY = vY+0.001f;
				if(maxZ!=maxZ || vZ>maxZ) maxZ = vZ+0.001f;
				dataBuffer.push_back(vX);
				dataBuffer.push_back(vY);
				dataBuffer.push_back(vZ);
				// Put the vertex data into the vertex buffer
				if(lenVertexTextures>0)
				{
					dataBuffer.push_back(vertexTextures[faceKey.vertexTextureIndex*2+0]);
					dataBuffer.push_back(vertexTextures[faceKey.vertexTextureIndex*2+1]);
				}
				if(lenVertexNormals>0)
				{
					dataBuffer.push_back(vertexNormals[faceKey.vertexNormalIndex*3+0]);
					dataBuffer.push_back(vertexNormals[faceKey.vertexNormalIndex*3+1]);
					dataBuffer.push_back(vertexNormals[faceKey.vertexNormalIndex*3+2]);
				}
			}
			else
			{
				// Store the existing vertex index in the new index buffer
				o->indecies[v] = findFace->second;
			}
		}
		delete [] objectIndecies;
		this->objects.push_back(o);
	}
	this->bounds->boxCenter[0] = (maxX+minX)/2.f;
	this->bounds->boxCenter[1] = (maxY+minY)/2.f;
	this->bounds->boxCenter[2] = (maxZ+minZ)/2.f;
	this->bounds->boxHalfSize[0] = this->bounds->boxCenter[0]-minX;
	this->bounds->boxHalfSize[1] = this->bounds->boxCenter[1]-minY;
	this->bounds->boxHalfSize[2] = this->bounds->boxCenter[2]-minZ;
	//cleanup
	delete [] vertexPositions;
	if(lenVertexTextures>0)
		delete [] vertexTextures;
	if(lenVertexNormals>0)
		delete [] vertexNormals;
	if(lenVertexColors>0)
		delete [] vertexColors;
}
StaticModel::~StaticModel()
{
	
}
void StaticModel::postload()
{
	// Load
	glGenVertexArrays(1, &this->vertexArrayID);
	glBindVertexArray(this->vertexArrayID);
	
	glGenBuffers(1, &this->vertexDataBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexDataBufferID);
	glBufferData(GL_ARRAY_BUFFER, this->dataBuffer.size()*sizeof(GLfloat), &this->dataBuffer[0], GL_STATIC_DRAW);
	
	GLfloat *colors = new GLfloat[temp_totalVertexCount*3];
	for(int i=0;i<temp_totalVertexCount*3;i++)
		colors[i] = (float)(std::rand()%100)/100;
	glGenBuffers(1, &tempColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->tempColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, temp_totalVertexCount*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	delete [] colors;

	for(StaticModelObject *object : this->objects)
	{
		glGenBuffers(1, &object->indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->numPrimitives*3*sizeof(GLuint), object->indecies, GL_STATIC_DRAW);
	}
}
AABB &StaticModel::getBounds()
{
	return *this->bounds;
}
void StaticModel::render(render::RenderManager &rManager, int shader)
{
	shaders::ShaderProgram *proc = rManager.useShader(shader);
	if(proc==0)
	{
		util::Globals::fatalError("Failed to find shader");
	}
	//glUniform1i(proc->getShaderLocation(true, SHADERVAR_material_map_Kd), 0);

	glBindVertexArray(this->vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexDataBufferID);

	// Push the vertex attributes
	proc->setVertexAttributePointer(SHADERVAR_vertex_position, 3, GL_FLOAT, GL_FALSE, dataBufferStride*sizeof(GLfloat), 0);
	proc->setVertexAttributePointer(SHADERVAR_vertex_texture, 2, GL_FLOAT, GL_FALSE, dataBufferStride*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	proc->setVertexAttributePointer(SHADERVAR_vertex_normal, 3, GL_FLOAT, GL_FALSE, dataBufferStride*sizeof(GLfloat), (void*)(this->dataBufferNormalsOffset*sizeof(GLfloat)));
	proc->setVertexAttributePointer(SHADERVAR_vertex_color, 3, GL_FLOAT, GL_FALSE, dataBufferStride*sizeof(GLfloat), (void*)(this->dataBufferColorsOffset*sizeof(GLfloat)));
	
	for(StaticModelObject *object : this->objects)
	{
		if(object->mtlAsset!=0)
			proc->setMaterial({object->mtlAsset, object->materialId});

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBufferID);
		glDrawElements(GL_TRIANGLES, object->numPrimitives*3, GL_UNSIGNED_INT, 0);
	}
}
void StaticModel::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".obj] " << this->dataBuffer.size()/this->dataBufferStride << " verticies by " << this->dataBufferStride << " attributes (" << this->dataBuffer.size() << " dbuf size), " << this->objects.size() << " objects:" << std::endl;
	for(render::StaticModelObject *o : this->objects)
		ost << *o << std::endl;
}
std::ostream &operator<<(std::ostream &ost, const render::StaticModelObject &o)
{
	return ost << "	" << o.name << ": Material [" << o.mtlAsset << ":" << o.materialId << "]" << ", Shading: " << o.s << ", " << o.numPrimitives << " primitives (" << (o.numPrimitives*3) << " verticies) ";
}
