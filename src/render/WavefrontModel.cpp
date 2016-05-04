#include "WavefrontModel.hpp"

#include <istream>
#include <iostream>
#include <unordered_map>
#include "util/StreamUtils.hpp"

using namespace render;
using namespace util::StreamUtils;

WavefrontModel::WavefrontModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));

	int dataBufferStride = 0;
	int dataBufferTexturesOffset = 0;
	int dataBufferNormalsOffset = 0;
	int dataBufferVertexCount = 0;
	int dataBufferFaceCount = 0;
	int vertexStride = 1;
	int vertexTexturesOffset = 0;
	int vertexNormalsOffset = 0;
	std::vector<float> dataBuffer;//[[v[0], v[1], v[2], vt[0], vt[1], vn[0], vn[1], vn[2]]...]
	std::unordered_map<int[3], int> assocMap;//[v index, vt index, vn index] : [faceIndex]

	//v
	int lenv = readInt(fp);
	float *vertexPosiions = new float[lenv*3];
	fp.read((char *)vertexPosiions, sizeof(float)*lenv);
	//vt
	int lenvt = readInt(fp);
	float *vertexTextures = new float[lenvt*2];
	fp.read((char *)vertexTextures, sizeof(float)*lenvt);
	//vn
	int lenvn = readInt(fp);
	float *vertexNormals = new float[lenvn*3];
	fp.read((char *)vertexNormals, sizeof(float)*lenvn);
	//enable t/n
	bool enableTextures = readBool(fp);
	bool enableNormals = readBool(fp);
	if(enableTextures)//enableTextures
	{
		dataBufferStride += 2;
		dataBufferTexturesOffset = 2;
		vertexStride += 1;
		vertexTexturesOffset = 1;
	}
	if(enableNormals)//enableNormals
	{
		dataBufferStride += 3;
		dataBufferNormalsOffset = 3+dataBufferTexturesOffset;
		vertexStride += 1;
		vertexNormalsOffset = vertexTexturesOffset+1;
	}
	//objects
	int len = readInt(fp);
	int temp[3];
	int currentVertexIndex = 0;
	for(int i=0;i<len;i++)
	{
		currentVertexIndex = 0;
		WavefrontObject *o = new WavefrontObject;
		// Load the wavefront object
		o->name = readString(fp);
		o->usemtl = readString(fp);
		o->s = readBool(fp);
		o->numPrimitives = readInt(fp);
		int *objectIndecies = new int[o->numPrimitives*3*vertexStride];
		o->indecies = new int[o->numPrimitives*3];
		fp.read((char *)o->indecies, sizeof(int)*o->numPrimitives*3);
		// Store the faces
		for(int v=0;v<o->numPrimitives*3;v++)
		{
			// Load the vertex indexes for the components of the vertex
			temp[0] = objectIndecies[v*vertexStride];
			temp[1] = (enableTextures ? objectIndecies[v*vertexStride+vertexTexturesOffset] : -1);
			temp[2] = (enableNormals ? objectIndecies[v*vertexStride+vertexNormalsOffset] : -1);

			std::unordered_map<int[3],int>::const_iterator findFace = assocMap.find(temp);
			if(findFace==assocMap.end())//If vertex is not already associated
			{
				// Associate the store the new index
				assocMap[temp] = currentVertexIndex;
				o->indecies[v] = currentVertexIndex;
				currentVertexIndex++;
				// Put the vertex data into the vertex buffer
				dataBuffer.push_back(vertexPosiions[temp[0]*3+0]);
				dataBuffer.push_back(vertexPosiions[temp[0]*3+1]);
				dataBuffer.push_back(vertexPosiions[temp[0]*3+2]);
				if(enableTextures)
				{
					dataBuffer.push_back(vertexTextures[temp[1]*2+0]);
					dataBuffer.push_back(vertexTextures[temp[1]*2+1]);
				}
				if(enableNormals)
				{
					dataBuffer.push_back(vertexNormals[temp[2]*3+0]);
					dataBuffer.push_back(vertexNormals[temp[2]*3+1]);
					dataBuffer.push_back(vertexNormals[temp[2]*3+2]);
				}
			}
			else
			{
				// Store the existing vertex index in the new index buffer
				o->indecies[v] = assocMap[temp];
			}
		}
	}
}
WavefrontModel::~WavefrontModel()
{
	
}
void WavefrontModel::postload()
{
	for(WavefrontObject &obj : this->objects)
	{
		//Allocate index buffer
		//push index data (fixed, size)
	}
}
