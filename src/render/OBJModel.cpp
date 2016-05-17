#include <istream>
#include <iostream>
#include <unordered_map>
#include "util/StreamUtils.hpp"
#include "util/gl.h"
#include "render/RenderManager.hpp"

#include "OBJModel.hpp"

using namespace render;
using namespace util::StreamUtils;

OBJModel::OBJModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));

	
	// int dataBufferStride = 0; - Field
	dataBufferStride = 3;
	dataBufferTexturesOffset = 0;
	dataBufferNormalsOffset = 0;
	int vertexStride = 1;
	int vertexTexturesOffset = 0;
	int vertexNormalsOffset = 0;
	//std::vector<float> dataBuffer;//[[v[0], v[1], v[2], vt[0], vt[1], vn[0], vn[1], vn[2]]...]
	std::unordered_map<struct FaceKey, int, FaceKeyHasher> assocMap;//[v index, vt index, vn index] : [faceIndex]
	
	//v
	int lenVertexPositions = readInt(fp);
	float *vertexPositions = new float[lenVertexPositions*3];
	fp.read((char *)vertexPositions, sizeof(float)*lenVertexPositions*3);
	//vt
	int lenVertexTextures = readInt(fp);
	float *vertexTextures = new float[lenVertexTextures*2];
	if(lenVertexTextures>0)//enableTextures
	{
		fp.read((char *)vertexTextures, sizeof(float)*lenVertexTextures*2);
		dataBufferStride += 2;
		dataBufferTexturesOffset = 2;
		vertexStride += 1;
		vertexTexturesOffset = 1;
	}
	//vn
	int lenVertexNormals = readInt(fp);
	float *vertexNormals = new float[lenVertexNormals*3];
	if(lenVertexNormals>0)//enableNormals
	{
		fp.read((char *)vertexNormals, sizeof(float)*lenVertexNormals*3);
		dataBufferStride += 3;
		dataBufferNormalsOffset = 3+dataBufferTexturesOffset;
		vertexStride += 1;
		vertexNormalsOffset = vertexTexturesOffset+1;
	}
	//objects
	int lenObjects = readInt(fp);
	struct FaceKey faceKey;
	int currentVertexIndex = 0;
	for(int i=0;i<lenObjects;i++)
	{
		OBJObject *o = new OBJObject;
		// Load the wavefront object
		o->name = readString(fp);
		o->usemtl = readString(fp);
		o->s = readBool(fp);
		o->numPrimitives = readInt(fp);
		int numVerticies = o->numPrimitives*3;
		int *objectIndecies = new int[numVerticies*vertexStride];
		o->indecies = new GLuint[numVerticies];
		fp.read((char *) objectIndecies, sizeof(int)*numVerticies*vertexStride);
		// Store the faces
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
				// Put the vertex data into the vertex buffer
				dataBuffer.push_back(vertexPositions[faceKey.vertexPositionIndex*3+0]);
				dataBuffer.push_back(vertexPositions[faceKey.vertexPositionIndex*3+1]);
				dataBuffer.push_back(vertexPositions[faceKey.vertexPositionIndex*3+2]);
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
		this->objects.push_back(o);
	}
}
OBJModel::~OBJModel()
{
	
}
void OBJModel::postload()
{
	// Load
	glGenVertexArrays(1, &this->vertexArrayID);
	glBindVertexArray(this->vertexArrayID);
	
	glGenBuffers(1, &this->vertexDataBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexDataBufferID);
	glBufferData(GL_ARRAY_BUFFER, this->dataBuffer.size()*sizeof(GLfloat), &this->dataBuffer[0], GL_STATIC_DRAW);

	for(OBJObject *object : this->objects)
	{
		glGenBuffers(1, &object->indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->numPrimitives*3*sizeof(GLuint), object->indecies, GL_STATIC_DRAW);
	}
}
void OBJModel::render(render::RenderManager &rManager, GLuint shaderVertexPositionID)
{
	glBindVertexArray(this->vertexArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexDataBufferID);
	glEnableVertexAttribArray(shaderVertexPositionID);
	glVertexAttribPointer(shaderVertexPositionID, 3, GL_FLOAT, GL_FALSE, dataBufferStride*sizeof(GLfloat), 0);
	
	for(OBJObject *object : this->objects)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBufferID);
		glDrawElements(GL_TRIANGLES, object->numPrimitives*3, GL_UNSIGNED_INT, 0);
	}
}
void OBJModel::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".obj] " << this->dataBuffer.size()/this->dataBufferStride << " verticies by " << this->dataBufferStride << " attributes (" << this->dataBuffer.size() << " dbuf size), " << this->objects.size() << " objects:" << std::endl;
	for(render::OBJObject *o : this->objects)
		ost << *o << std::endl;
}
std::ostream &operator<<(std::ostream &ost, const render::OBJObject &o)
{
	return ost << "	" << o.name << ": Material " << o.usemtl << ", Shading: " << o.s << ", " << o.numPrimitives << " primitives (" << (o.numPrimitives*3) << " verticies) ";
}
