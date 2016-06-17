#include <iostream>
#include "util/StreamUtils.hpp"

//#ifdef ENABLE_DEBUG_RENDER_MD5JOINT
#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "util/gl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "util/AssetManager.hpp"
#include "render/MaterialLibrary.hpp"
#include "render/DDSImage.hpp"
//#endif

#include "SkeletalModel.hpp"

using namespace util;
using namespace util::StreamUtils;
using namespace render;

void render::calculateQuaternionW(glm::quat &q)
{
	float t = 1.0f - (q.x*q.x)-(q.y*q.y)-(q.z*q.z);
	if(t<0.0f)
	{
		q.w = 0.f;
	}
	else
	{
		q.w = -std::sqrt(t);
	}
}

SkeletalModel::SkeletalModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));
	//load joints and meshes
	int numJoints = readInt(fp);
	for(int j=0;j<numJoints;j++)
	{
		MD5Joint joint;
		MD5Bone bone;
		// Read the joint data
		joint.index = j;
		joint.name = readString(fp);
		joint.parent = readInt(fp);
		bone.pos.x = readFloat(fp);
		bone.pos.y = readFloat(fp);
		bone.pos.z = readFloat(fp);
		bone.ori.x = readFloat(fp);
		bone.ori.y = readFloat(fp);
		bone.ori.z = readFloat(fp);
		calculateQuaternionW(bone.ori);
		joints.push_back(joint);
		bindPoseSkeleton.push_back(bone);
	}
	int numMeshes = readInt(fp);
	//meshes.append((shader_name, numverts, verts, numtris, tris, numweights, weights))
	for(int i=0;i<numMeshes;i++)
	{
		MD5Mesh mesh;
		mesh.mtlAssetId = readInt(fp);
		mesh.materialId = readInt(fp);
		int numverts = readInt(fp);
		//verts.append((vertIndex, tex, startWeight, countWeight))
		for(int i=0;i<numverts;i++)
		{
			MD5Vertex vertex;
			vertex.index = readInt(fp);
			mesh.textureUVs.push_back(readFloat(fp));
			mesh.textureUVs.push_back(readFloat(fp));
			vertex.startWeight = readInt(fp);
			vertex.countWeight = readInt(fp);
			mesh.verts.push_back(vertex);
		}
		int numtris = readInt(fp);
		//tris.append((triIndex, vertIndex0, vertIndex1, vertIndex2))
		for(int i=0;i<numtris;i++)
		{
			readInt(fp);
			mesh.indecies.push_back(readInt(fp));
			mesh.indecies.push_back(readInt(fp));
			mesh.indecies.push_back(readInt(fp));
		}
		int numweights = readInt(fp);
		//weights.append((weightIndex, joint, bias, pos))
		for(int i=0;i<numweights;i++)
		{
			MD5Weight weight;
			weight.index = readInt(fp);
			weight.joint = readInt(fp);
			weight.bias = readFloat(fp);
			weight.pos.x = readFloat(fp);
			weight.pos.y = readFloat(fp);
			weight.pos.z = readFloat(fp);
			mesh.weights.push_back(weight);
		}
		meshes.push_back(mesh);
	}
}
SkeletalModel::~SkeletalModel()
{
	
}
void SkeletalModel::render()
{
	//Select shader
	//FOR EACH MESH
		//Calculate vertex positions
		//Update vertex buffers
		
		//Push shader variables
		//draw
}
void SkeletalModel::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".md5mesh]";
	for(const render::MD5Joint &joint : this->joints)
	{
		ost << joint;
	}
}
void SkeletalModel::postload()
{

	glGenVertexArrays(1, &this->vertexArrayID);
	glBindVertexArray(this->vertexArrayID);
	for(render::MD5Mesh &mesh : this->meshes)
	{
		glGenBuffers(1, &mesh.vertexBufferID);
		glGenBuffers(1, &mesh.vertexTextureBufferID);
		glGenBuffers(1, &mesh.indexBufferID);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexTextureBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh.textureUVs.size()*sizeof(GLfloat), &mesh.textureUVs[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indecies.size()*sizeof(GLuint), &mesh.indecies[0], GL_STATIC_DRAW);



		GLfloat *vertexColorData = new GLfloat[mesh.verts.size()*3*sizeof(GLfloat)];
		for(int v=0;v<(int)mesh.verts.size();v++)
		{
			vertexColorData[v*3+0] = (float)(std::rand()%1000)/1000;
			vertexColorData[v*3+1] = (float)(std::rand()%1000)/1000;
			vertexColorData[v*3+2] = (float)(std::rand()%1000)/1000;
		}

		glGenBuffers(1, &mesh.vertexColorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexColorBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh.verts.size()*3*sizeof(GLfloat), vertexColorData, GL_STATIC_DRAW);
	}
}
//#ifdef ENABLE_DEBUG_RENDER_MD5JOINT
void SkeletalModel::renderSkeleton(render::RenderManager &manager, const Skeleton &skeleton)
{
	manager.disableDepth();
	manager.useShader(SHADER_solidColor);
	shaders::ShaderProgram *shader = shaders::ShaderProgram::getShader(SHADER_solidColor);
	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	float f = 0;
	for(MD5Joint &joint : this->joints)
	{
		if(joint.parent>=0)
		{
			glUniform4f(loc, 1.f, 0.0f, 0.0f, 1.0f);
			BasicShapes::drawLine(skeleton[joint.index].pos, skeleton[joint.parent].pos, vploc);
		}
		f += 1/(float)this->joints.size();
	}
	manager.enableDepth();
}
void SkeletalModel::renderWeights(render::RenderManager &manager, const Skeleton &skeleton)
{
	manager.disableDepth();
	float f = 0;
	manager.useShader(SHADER_solidColor);
	shaders::ShaderProgram *shader = shaders::ShaderProgram::getShader(SHADER_solidColor);
	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	for(MD5Mesh &mesh : this->meshes)
	{
		for(MD5Weight &weight : mesh.weights)
		{
			// Weight to Joint
			MD5Joint &joint = this->joints[weight.joint];
			glUniform4f(loc, 0.f, 1.0f, 0.f, 1.0f);

			BasicShapes::drawLine(skeleton[joint.index].pos, skeleton[joint.index].pos+skeleton[joint.index].ori*weight.pos, vploc);
		}
		f+=1/(float)this->meshes.size();
	}
	manager.enableDepth();
}
//#endif
void SkeletalModel::render(render::RenderManager &manager)
{
	this->render(manager, this->bindPoseSkeleton);
}
void SkeletalModel::render(render::RenderManager &rManager, const Skeleton &skeleton)
{
	glBindVertexArray(this->vertexArrayID);
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_fuzzyModel);
	//GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
	//glUniform4f(loc, 0.f, 0.0f, 1.f, 1.0f);
	for(MD5Mesh &mesh : this->meshes) {
		this->render(rManager, skeleton, mesh, 0);
	}
}
void SkeletalModel::render(render::RenderManager &manager, const Skeleton &skeleton, MD5Mesh &mesh, const Material *material)
{
	GLfloat *vertexData = new GLfloat[mesh.verts.size()*3*sizeof(GLfloat)];
	for(int v=0;v<(int)mesh.verts.size();v++)
	{
		MD5Vertex &vertex = mesh.verts[v];
		glm::vec3 vect;
		vect.x = 0;
		vect.y = 0;
		vect.z = 0;
		for(int w=0;w<vertex.countWeight;w++)
		{
			MD5Weight &weight = mesh.weights[vertex.startWeight+w];
			const MD5Bone &bone = skeleton[weight.joint];
			vect += ( bone.pos + bone.ori*weight.pos)*weight.bias;
		}
		vertexData[v*3+0] = vect.x;
		vertexData[v*3+1] = vect.y;
		vertexData[v*3+2] = vect.z;
	}
	// vertex position
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size()*3*sizeof(GLfloat), vertexData, GL_DYNAMIC_DRAW);
	shaders::ShaderProgram *shader = shaders::ShaderProgram::getShader(SHADER_fuzzyModel);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	glEnableVertexAttribArray(vploc);
	glVertexAttribPointer(vploc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint colorLoc = shader->getShaderLocation(false, SHADERVAR_vertex_color);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexColorBufferID);
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glActiveTexture(GL_TEXTURE0);
	//int materialLocation = ((MaterialLibrary *)util::AssetManager::getAssetManager()->getAsset(mesh.mtlAssetId))->getMaterial(mesh.materialId)->map_Kd;
	//((DDSImage *)util::AssetManager::getAssetManager()->getAsset(materialLocation))->bindTexture();

	//glUniform1i(shaders::program_modelTexture_myTextureSampler, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexTextureBufferID);
	//glEnableVertexAttribArray(uvloc);
	//glVertexAttribPointer(uvloc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indecies.size()*sizeof(GLuint), &mesh.indecies[0], GL_DYNAMIC_DRAW);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}
std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint)
{
	ost << "\t\"" << joint.name << "\" " << joint.parent << std::endl;
	return ost;
}
std::ostream &operator<<(std::ostream &ost, const render::MD5Mesh &mesh)
{
	ost << "\t" << "[Material#" << mesh.materialId << ":" << mesh.materialId << "]" << mesh.verts.size() << " verticies, " << mesh.indecies.size() << " triangles, " << mesh.weights.size() << " weights";
	return ost;
}
std::ostream &operator<<(std::ostream &ost, const render::MD5Vertex &vert)
{
	ost << vert.index << ",  " << vert.startWeight << "-" << (vert.startWeight+vert.countWeight) << " (" << vert.countWeight << " weights)";
	return ost;
}
std::ostream &operator<<(std::ostream &ost, const render::MD5Weight &weight)
{
	ost << weight.index << " " << weight.joint << " " << weight.bias << " ( " << weight.pos.x << " " << weight.pos.y << " " << weight.pos.z << " )";
	return ost;
}
