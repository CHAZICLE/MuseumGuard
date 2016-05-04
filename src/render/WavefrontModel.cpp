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
	std::vector<float> dataBuffer;//[[v[0], v[1], v[2], vt[0], vt[1], vn[0], v[1], v[2]]...]
	std::unordered_map<int[3], int> assocMap;//[v index, vt index, vn index] : [faceIndex]

	//v
	int lenv = readInt(fp);
	float *vertexPosiions = new float[lenv*3];
	fp.read((char *)vertexPosiions sizeof(float)*lenv);
	//vt
	int lenvt = readInt(fp);
	float *vt = new float[lenvt*2];
	fp.read((char *)vt, sizeof(float)*lenvt);
	//vn
	int lenvn = readInt(fp);
	float *vn = new float[lenvn*3];
	fp.read((char *)vn, sizeof(float)*lenvn);
	//enable t/n
	bool enableTextures = readBool(fp);
	bool enableNormals = readBool(fp);
	if(enableTextures)//enableTextures
	{
		dataBufferStride += 2;
		dataBufferTexturesOffset = 2;
		vertexStride += 1;
		vertexTexturesOffset++;
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
	for(int i=0;i<len;i++)
	{
		// Load the wavefront object
		std::string object_name = readString(fp);
		bool usemtl = readBool(fp);
		bool s = readBool(fp);
		int numVerticies = readInt(fp);
		int *vertexIndicies = new int[numVerticies*vertexStride];
		fp.read((char *)vertexIndicies, sizeof(int)*numVerticies*vertexStride);
		// Store the faces
		for(int v=0;v<numVerticies;v++)
		{
			// Load the vertex indexes for the components of the vertex
			temp[0] = vertexIndicies[v*vertexStride];
			temp[1] = (enableTextures ? vertexIndicies[v*vertexStride+vertexTexturesOffset] : -1);
			temp[2] = (enableNormals ? vertexIndicies[v*vertexStride+vertexNormalsOffset] : -1);

			std::unordered_map<int[3],int>::const_iterator findFace = assocMap.find(temp);
			if(findFace==assocMap.end())
			{
				int faceAssoc = assocMap[temp];
				dataBuffer.push_back(vertexPosiions[temp[0]*3+0]);
			}
			else
			{
				std::cout << findFace->second << std::endl;
			}
			faceIndexBuffer[f] = assocMap[temp]
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
