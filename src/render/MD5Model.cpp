#include <iostream>
#include "util/StreamUtils.hpp"

#include "MD5Model.hpp"

using namespace util;
using namespace util::StreamUtils;
using namespace render;

MD5Model::MD5Model(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));
	//load joints and meshes
	int numJoints = readInt(fp);
	for(int j=0;j<numJoints;j++)
	{
		MD5Joint joint;
		// Read the joint data
		joint.name = readString(fp);
		joint.parent = readInt(fp);
		joint.pos = readVec3f(fp);
		joint.ori = readVec3f(fp);
		joints.push_back(joint);
	}
	int numMeshes = readInt(fp);
	//meshes.append((shader_name, numverts, verts, numtris, tris, numweights, weights))
	for(int i=0;i<numMeshes;i++)
	{
		MD5Mesh mesh;
		mesh.shader_name = readString(fp);
		int numverts = readInt(fp);
		//verts.append((vertIndex, tex, startWeight, countWeight))
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
		//tris.append((triIndex, vertIndex0, vertIndex1, vertIndex2))
		for(int i=0;i<numtris;i++)
		{
			MD5Primitive tri = {readInt(fp), readInt(fp), readInt(fp), readInt(fp)};
			mesh.tris.push_back(tri);
		}
		int numweights = readInt(fp);
		//weights.append((weightIndex, joint, bias, pos))
		for(int i=0;i<numweights;i++)
		{
			MD5Weight weight;
			weight.index = readInt(fp);
			weight.joint = readInt(fp);
			weight.bias = readFloat(fp);
			weight.pos = readVec3f(fp);
			mesh.weights.push_back(weight);
			//std::cout << weight.index << ":" << weight.pos.x << ", " << weight.pos.y << ", " << weight.pos.z << std::endl;
		}
		meshes.push_back(mesh);
	}
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
void MD5Model::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".md5mesh]";
	for(const render::MD5Joint &joint : this->joints)
	{
		ost << joint;
	}
}
void MD5Model::postload()
{

}
std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint)
{
	ost << "\t\"" << joint.name << "\" " << joint.parent << " ( " << joint.pos.x << ", " << joint.pos.y << ", " << joint.pos.z << " ) ( " << joint.ori.x << ", " << joint.ori.y << ", " << joint.ori.z << " )" << std::endl;
	return ost;
}
