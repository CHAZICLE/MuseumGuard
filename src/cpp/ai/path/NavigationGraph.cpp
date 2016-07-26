#include "util/gl.h"
#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "util/StreamUtils.hpp"

#include "util/Globals.hpp"

#include "NavigationGraph.hpp"

using namespace ai::path;
using namespace util::StreamUtils;
using namespace render;

NavigationGraph::NavigationGraph(int assetId, std::istream &fp) : Asset(assetId)
{
	setName(readString(fp));
	this->numGroups = readInt(fp);
	this->groupCounts = new int[this->numGroups];
	std::fill(this->groupCounts, this->groupCounts+this->numGroups, 0);
	this->numNodes = readInt(fp);
	this->groupMasks = new int[this->numNodes];
	this->nodes = new PathNode[this->numNodes];
	for(int i=0;i<this->numNodes;i++)
	{
		PathNode &n = this->nodes[i];
		n.id = i;
		this->groupMasks[i] = readInt(fp);
		for(int j=0;j<this->numGroups;j++)
		{
			if((1<<j)&this->groupMasks[i])
				this->groupCounts[j]++;
		}
		n.position.x = readFloat(fp);
		n.position.y = readFloat(fp);
		n.position.z = readFloat(fp);
	}
	for(int j=0;j<this->numGroups;j++)
		PRINT_DEBUG("ng:" << j << ":" << this->groupCounts[j]);
	// Create node links
	numNodeLinks = readInt(fp);
	unsigned int tmp;
	for(int i=0;i<numNodeLinks;i++)
	{
		PathNodeLink *pnLink = new PathNodeLink;
		tmp = readInt(fp);
		if(tmp<0 || tmp>=this->numNodes)
			util::Globals::fatalError("Node link a outside range "+std::to_string(tmp)+" vs "+std::to_string(this->numNodes));
		pnLink->a = &this->nodes[tmp];
		tmp = readInt(fp);
		if(tmp<0 || tmp>=this->numNodes)
			util::Globals::fatalError("Node link b outside range");
		pnLink->id = i;
		pnLink->b = &this->nodes[tmp];
		pnLink->dist = glm::distance(pnLink->a->position, pnLink->b->position);
		pnLink->a->links.push_back(pnLink);
		pnLink->b->links.push_back(pnLink);
	}
}
NavigationGraph::~NavigationGraph()
{
	std::set<struct PathNodeLink *> links;
	for(unsigned int i=0;i<this->numNodes;i++)
	{
		auto *n = &this->nodes[i];
		for(auto &nLink : n->links)
			links.insert(nLink);
	}
	for(auto &n : links)
		delete n;
	delete [] this->nodes;
}
void NavigationGraph::write(std::ostream &ost) const
{
}
void NavigationGraph::postload()
{
	glGenVertexArrays(1, &this->vertexArrayId);
	glBindVertexArray(this->vertexArrayId);


	// Add vertex positions
	GLfloat *vertexPositionData = new GLfloat[this->numNodes*3];
	vertexColorData = new GLfloat[this->numNodes*3];
	for(unsigned int i=0;i<this->numNodes;i++)
	{
		PathNode &n = this->nodes[i];
		vertexPositionData[i*3+0] = n.position.x;
		vertexPositionData[i*3+1] = n.position.y;
		vertexPositionData[i*3+2] = n.position.z;
	}
	glGenBuffers(1, &this->vertexPositionBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexPositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numNodes*3, vertexPositionData, GL_STATIC_DRAW);

	glGenBuffers(1, &this->vertexColorBufferID);

	GLuint *vertexIndexBuffer = new GLuint[this->numNodeLinks*2];
	int j = 0;
	for(unsigned int i=0;i<this->numNodes;i++)
	{
		for(auto l : this->nodes[i].links)
		{
			if(l->a==&this->nodes[i])
			{
				vertexIndexBuffer[j*2+0] = l->a->id;
				vertexIndexBuffer[j*2+1] = l->b->id;
				j++;
			}
		}
	}
	glGenBuffers(1, &this->indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->numNodeLinks*2, vertexIndexBuffer, GL_STATIC_DRAW);

	linkColorData = new GLfloat[this->numNodeLinks*3];
	glGenBuffers(1, &this->indexColorBufferID);
}
void NavigationGraph::render(render::RenderManager &rManager)
{
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_fuzzyModel);

	glBindVertexArray(this->vertexArrayId);
	//glEnable(GL_BLEND);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexPositionBufferId);
	shader->setVertexAttributePointer(SHADERVAR_vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	for(unsigned int i=0;i<this->numNodes;i++)
	{
		PathNode &node = this->nodes[i];
		glm::vec3 vertexColors;
		if(node.current)
			vertexColors = {1.0f, 0.f, 0.f};
		else if(node.closed)
			vertexColors = {1.0f, 1.f, 0.f};
		else if(node.open)
			vertexColors = {0.0f, 1.f, 0.f};
		else
			vertexColors = {0.0f, 0.f, 1.f};
		*(vertexColorData+i*3+0) = vertexColors[0];
		*(vertexColorData+i*3+1) = vertexColors[1];
		*(vertexColorData+i*3+2) = vertexColors[2];
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numNodes*3, this->vertexColorData, GL_DYNAMIC_DRAW);
	shader->setVertexAttributePointer(SHADERVAR_vertex_color, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	glPointSize(4.f);
	glDrawArrays(GL_POINTS, 0, this->numNodes*3);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexPositionBufferId);
	shader->setVertexAttributePointer(SHADERVAR_vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	// Draw the links
	//int j = 0;
	//for(unsigned int i=0;i<this->numNodes;i++)
	//{
	//	for(auto l : this->nodes[i].links)
	//	{
	//		if(l->a==&this->nodes[i])
	//		{
	//			glm::vec3 linkColor;// = *(glm::vec3 *)&this->linkColorData[j*3];
	//			if(l->a->current && l->b->current)
	//				linkColor = {1.0f, 0.0f, 0.0f};
	//			else if((l->a->current && l->b->closed) && (l->b->current && l->a->closed))
	//				linkColor = {1.0f, 1.0f, 0.0f};
	//			else if((l->a->closed && l->b->open) && (l->b->closed && l->a->open))
	//				linkColor = {0.0f, 1.0f, 0.0f};
	//			else
	//				linkColor = {0.0f, 0.0f, 1.0f};
	//			*(this->linkColorData+j*3) = linkColor[0];
	//			j++;
	//		}
	//	}
	//}
	//glBindBuffer(GL_ARRAY_BUFFER, this->indexColorBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*this->numNodeLinks*3, this->linkColorData, GL_DYNAMIC_DRAW);
	//shader->setVertexAttributePointer(SHADERVAR_vertex_color, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
	glDrawElements(GL_LINES, this->numNodeLinks*2, GL_UNSIGNED_INT, 0);
	//

	//std::cout << this->vertexArrayId << ":" << this->vertexPositionBufferId << ", " << this->vertexColorBufferId << std::endl;

//	rManager.M = glm::mat4(1.0f);
//	rManager.markMDirty();
//	shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
//
//	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
//	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);
//
//	
//	for(int i=0;i<this->numNodes;i++)
//	//for(std::vector<struct PathNode *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
//	{
//		//struct PathNode *node = *it;
//		PathNode &node = this->nodes[i];
//		// Draw a point for the node
//		if(node.current)
//			glUniform4f(loc, 1.0f, 0.f, 0.f, 1.f);
//		else if(node.open)
//			glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
//		else if(node.closed)
//			glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
//		else
//			continue;
//			//glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
//		rManager.M = glm::translate(glm::mat4(1.0f), node.position);
//		rManager.markMDirty();
//		rManager.setShaderMatricies(*shader);
//		BasicShapes::drawPoint(4,vploc);
//
//		// Draw all node links
//		rManager.M = glm::mat4(1.0f);
//		rManager.markMDirty();
//		rManager.setShaderMatricies(*shader);
//		for(std::vector<struct PathNodeLink *>::iterator j = node.links.begin(); j != node.links.end(); j++)
//		{
//			struct PathNodeLink *nodeLnk = *j;
//			if(nodeLnk->a->current && nodeLnk->b->current)
//				glUniform4f(loc, 1.0f, 0.f, 0.f, 1.f);
//			else if((nodeLnk->a->closed && (nodeLnk->b->current || nodeLnk->b->closed)) || (nodeLnk->b->closed && (nodeLnk->a->current || nodeLnk->a->closed)))
//				glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
//			else if((nodeLnk->a->open && nodeLnk->b->closed) || (nodeLnk->b->open && nodeLnk->a->closed))
//				glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
//			else if(
//				((nodeLnk->a->current || nodeLnk->a->open || nodeLnk->a->closed) && (!nodeLnk->b->current && !nodeLnk->b->open && !nodeLnk->b->closed)) || 
//				((nodeLnk->b->current || nodeLnk->b->open || nodeLnk->b->closed) && (!nodeLnk->a->current && !nodeLnk->a->open && !nodeLnk->a->closed))
//			       )
//				glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
//			else
//				continue;
//			//{
//			//	if(nodeLnk->a->closed && nodeLnk->b->closed)
//			//		glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
//			//	else if(nodeLnk->a->open && nodeLnk->b->open)
//			//		glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
//			//	else
//			//		glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
//			//}
//			BasicShapes::drawLine(nodeLnk->a->position, nodeLnk->b->position, vploc);
//		}
//	}
//	glDisable(GL_BLEND);
}
PathNode *NavigationGraph::getRandomNode(int group)
{
	int r = std::rand()%this->groupCounts[group];
	int c = 0;
	for(int i=0;i<this->numNodes;i++)
	{
		if(this->groupMasks[i]&(1<<group))
		{
			if(c==r)
				return &this->nodes[i];
			c++;
		}
	}
	return 0;
}
PathNode *NavigationGraph::getNearestPathNode(glm::vec3 position, int mask)
{
	PathNode *finalPathNode = 0;
	float min = std::numeric_limits<float>::max();
	for(int i=0;i<this->numNodes;i++)
	{
		PathNode *n = &this->nodes[i];
		if(this->groupMasks[i]&mask)
		{
			float f = glm::length2(n->position-position);
			if(min>f)
			{
				finalPathNode = n;
				min = f;
			}
		}
	}
	return finalPathNode;
}
PathNode *NavigationGraph::getNearestPathNode(glm::vec3 position)
{
	PathNode *finalPathNode = 0;
	float min = std::numeric_limits<float>::max();
	for(int i=0;i<this->numNodes;i++)
	{
		PathNode *n = &this->nodes[i];
		float f = glm::length2(n->position-position);
		if(min>f)
		{
			finalPathNode = n;
			min = f;
		}
	}
	return finalPathNode;
}
PathNode *NavigationGraph::getNearestPathNodeFromPool(glm::vec3 position, std::set<int> &pool)
{
	PathNode *finalPathNode = 0;
	float min = std::numeric_limits<float>::max();
	for(int i=0;i<this->numNodes;i++)
	{
		PathNode *n = &this->nodes[i];
		if(pool.find(n->id)!=pool.end())
		{
			float f = glm::length2(n->position-position);
			if(min>f)
			{
				finalPathNode = n;
				min = f;
			}
		}
	}
	return finalPathNode;
}
int NavigationGraph::getPathNodeGroupMask(int id)
{
	return this->groupMasks[id];
}
