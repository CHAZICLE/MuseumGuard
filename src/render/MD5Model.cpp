#include <iostream>

#include "MD5Model.hpp"

using namespace render;

inline std::string readString(std::istream &fp)
{
	int len;
	fp.read((char *)&len, sizeof(int));
	char *buf = new char[len+1];
	fp.read(buf, len);
	buf[len] = '\0';
	return std::string(buf);
}
inline int readInt(std::istream &fp)
{
	int i;
	fp.read((char *)&i, sizeof(int));
	return i;
}
inline float readFloat(std::istream &fp)
{
	float i;
	fp.read((char *)&i, sizeof(float));
	return i;
}

#define READ_VEC3F(x) glm::vec3(readFloat(x), readFloat(x), readFloat(x))

MD5Model::MD5Model(std::istream &fp)
{
	readString(fp);
	//load joints and meshes
	int numJoints;
	int numMeshes;
	fp.read((char *)&numJoints, sizeof(int));
	fp.read((char *)&numMeshes, sizeof(int));
	std::cout << "numJoints=" << numJoints << ", numMeshes=" << numMeshes << std::endl;
	
	for(int j=0;j<numJoints;j++)
	{
		MD5Joint joint;
		// Read the joint data
		joint.name = readString(fp);
		fp.read((char *)&joint.parent, sizeof(int));
		joint.pos = READ_VEC3F(fp);
		joint.ori = READ_VEC3F(fp);
		joints.push_back(joint);
		//std::cout << joint;
	}
	for(int i=0;i<numMeshes;i++)
	{
		MD5Mesh mesh;
		mesh.shader_name = readString(fp);
		int numverts = readInt(fp);
		for(int i=0;i<numverts;i++)
		{
			MD5Vertex vertex;
			vertex.index = readInt(fp);
			vertex.tex = glm::vec2(readFloat(fp), readFloat(fp));
			vertex.startWeight = readInt(fp);
			vertex.countWeight = readInt(fp);
			mesh.verts.push_back(vertex);
		}
		int numtris = readInt(fp);
		for(int i=0;i<numtris;i++)
		{
			MD5Tri tri = {readInt(fp), readInt(fp), readInt(fp), readInt(fp)};
			mesh.tris.push_back(tri);
		}
		int numweights = readInt(fp);
		for(int i=0;i<numweights;i++)
		{
			MD5Weight weight;
			weight.index = readInt(fp);
			weight.joint = readInt(fp);
			weight.bias = readFloat(fp);
			weight.pos = READ_VEC3F(fp);
			mesh.weights.push_back(weight);
		}
	}
	//Allocate vertex array objects and buffer objects
}
MD5Model::~MD5Model()
{
	
}
void MD5Model::render()
{
	//Select shader
	//FOR EACH MESH
		//Calculate vertex positions
		//Update vertex buffers
		
		//Push shader variables
		//draw
}
std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint)
{
	ost << "\t\"" << joint.name << "\" " << joint.parent << " ( " << joint.pos.x << ", " << joint.pos.y << ", " << joint.pos.z << " ) ( " << joint.ori.x << ", " << joint.ori.y << ", " << joint.ori.z << " )" << std::endl;
	return ost;
}
