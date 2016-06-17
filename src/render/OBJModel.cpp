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

#include "OBJModel.hpp"

using namespace render;
using namespace util::StreamUtils;

OBJModel::OBJModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));

	
	// int dataBufferStride = 0; - Field
	dataBufferStride = 3;
	dataBufferNormalsOffset = 0;
	dataBufferColorsOffset = 0;
	int vertexStride = 1;
	int vertexTexturesOffset = 0;
	int vertexNormalsOffset = 0;
	//std::vector<float> dataBuffer;//[[v[0], v[1], v[2], vt[0], vt[1], vn[0], vn[1], vn[2]]...]
	std::unordered_map<struct FaceKey, int, FaceKeyHasher> assocMap;//[v index, vt index, vn index] : [faceIndex]
	
	//vertex positions
	int lenVertexPositions = readInt(fp);
	float *vertexPositions = new float[lenVertexPositions*3];
	fp.read((char *)vertexPositions, sizeof(float)*lenVertexPositions*3);
	//vertex textures
	int lenVertexTextures = readInt(fp);
	float *vertexTextures = new float[lenVertexTextures*2];
	if(lenVertexTextures>0)//enableTextures
	{
		fp.read((char *)vertexTextures, sizeof(float)*lenVertexTextures*2);
		dataBufferStride += 2;
		vertexStride += 1;
		vertexTexturesOffset = 1;
	}
	//vertex normals
	int lenVertexNormals = readInt(fp);
	float *vertexNormals = new float[lenVertexNormals*3];
	if(lenVertexNormals>0)//enableNormals
	{
		fp.read((char *)vertexNormals, sizeof(float)*lenVertexNormals*3);
		dataBufferNormalsOffset = dataBufferStride;
		dataBufferStride += 3;
		vertexStride += 1;
		vertexNormalsOffset = vertexTexturesOffset+1;
	}
	//vertex colors
	int lenVertexColors = readInt(fp);
	float *vertexColors = new float[lenVertexColors*3];
	if(lenVertexColors>0)//enableNormals
	{
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
	for(int i=0;i<lenObjects;i++)
	{
		OBJObject *o = new OBJObject;
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
	
	GLfloat *colors = new GLfloat[temp_totalVertexCount*3];
	for(int i=0;i<temp_totalVertexCount*3;i++)
		colors[i] = (float)(std::rand()%100)/100;
	glGenBuffers(1, &tempColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->tempColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, temp_totalVertexCount*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);

	for(OBJObject *object : this->objects)
	{
		glGenBuffers(1, &object->indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->numPrimitives*3*sizeof(GLuint), object->indecies, GL_STATIC_DRAW);
	}
}
void OBJModel::render(render::RenderManager &rManager, int shader)
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
	
	for(OBJObject *object : this->objects)
	{
		if(object->mtlAsset!=0)
			proc->setMaterial({object->mtlAsset, object->materialId});

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
	return ost << "	" << o.name << ": Material [" << o.mtlAsset << ":" << o.materialId << "]" << ", Shading: " << o.s << ", " << o.numPrimitives << " primitives (" << (o.numPrimitives*3) << " verticies) ";
}
