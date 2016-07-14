#include "util/gl.h"
#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include <set>
#include "util/StreamUtils.hpp"

#include "util/Globals.hpp"

#include "NavigationGraph.hpp"

using namespace ai::path;
using namespace util::StreamUtils;
using namespace render;

NavigationGraph::NavigationGraph(int assetId, std::istream &fp) : Asset(assetId)
{
	std::srand(std::time(0));
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
	int numNodeLinks = readInt(fp);
	int tmp;
	for(int i=0;i<numNodeLinks;i++)
	{
		PathNodeLink *pnLink = new PathNodeLink;
		tmp = readInt(fp);
		if(tmp<0 || tmp>=this->numNodes)
			util::Globals::fatalError("Node link a outside range");
		pnLink->a = &this->nodes[tmp];
		tmp = readInt(fp);
		if(tmp<0 || tmp>=this->numNodes)
			util::Globals::fatalError("Node link b outside range");
		pnLink->b = &this->nodes[tmp];
		pnLink->dist = glm::distance(pnLink->a->position, pnLink->b->position);
		pnLink->a->links.push_back(pnLink);
		pnLink->b->links.push_back(pnLink);
	}
	vpd = new GLfloat[this->numNodes*3];
	vcd = new GLfloat[this->numNodes*3];
}
NavigationGraph::~NavigationGraph()
{
	std::set<struct PathNodeLink *> links;
	for(int i=0;i<this->numNodes;i++)
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
	//glGenVertexArrays(1, &this->vaid);
	//glBindVertexArray(this->vaid);

	//glGenBuffers(1, &this->vpbid);
	//glGenBuffers(1, &this->vcbid);
}
void NavigationGraph::render(render::RenderManager &rManager)
{
	//rManager.M = glm::mat4(1.0f);
	//rManager.markMDirty();
	//shaders::ShaderProgram *shader = rManager.useShader(SHADER_fuzzyModel);

	//glBindVertexArray(this->vaid);
	////glEnable(GL_BLEND);
	////rManager.M = glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 10.f));

	//// Add vertex positions
	//for(int i=0;i<this->numNodes;i++)
	//{
	//	PathNode &n = this->nodes[i];
	//	vpd[i*3+0] = n.position.x;
	//	vpd[i*3+1] = n.position.y;
	//	vpd[i*3+2] = n.position.z;
	//}
	//glBindBuffer(GL_VERTEX_ARRAY, this->vpbid);
	//glBufferData(GL_VERTEX_ARRAY, sizeof(GLfloat)*this->numNodes, vpd, GL_DYNAMIC_DRAW);
	//
	//glEnableVertexAttribArray(shader->getShaderLocation(false, SHADERVAR_vertex_position));
	//glVertexAttribPointer(shader->getShaderLocation(false, SHADERVAR_vertex_position), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	//for(int i=0;i<this->numNodes;i++)
	//{
	//	//PathNode &node = this->nodes[i];
	//	vcd[i*3+0] = 0.f;
	//	vcd[i*3+1] = 0.f;
	//	vcd[i*3+2] = 0.f;
	//	//if(node.current)
	//	//	vertexColors = {1.0f, 0.f, 0.f};
	//	//else if(node.closed)
	//	//	vertexColors = {1.0f, 1.f, 0.f};
	//	//else if(node.open)
	//	//	vertexColors = {0.0f, 1.f, 0.f};
	//	//else
	//	//	vertexColors = {0.0f, 0.f, 1.f};
	//}


	//glBindBuffer(GL_VERTEX_ARRAY, this->vcbid);
	//glBufferData(GL_VERTEX_ARRAY, sizeof(GLfloat)*this->numNodes*3, vcd, GL_DYNAMIC_DRAW);
	//std::cout << "T:" << shader->getShaderLocation(false, SHADERVAR_vertex_color) << std::endl;
	//glEnableVertexAttribArray(shader->getShaderLocation(false, SHADERVAR_vertex_position));
	//glVertexAttribPointer(shader->getShaderLocation(false, SHADERVAR_vertex_color), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);

	////std::cout << this->vertexArrayId << ":" << this->vertexPositionBufferId << ", " << this->vertexColorBufferId << std::endl;

	//glPointSize(10.f);

	//GLuint ind[] = {
	//	0,1,2
	//};
	//
	//glDrawElements(GL_POINTS, 3, GL_UNSIGNED_INT, ind);


	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);

	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	GLint loc = shader->getShaderLocation(true, SHADER_solidColor_solidColor);

	
	for(int i=0;i<this->numNodes;i++)
	//for(std::vector<struct PathNode *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		//struct PathNode *node = *it;
		PathNode &node = this->nodes[i];
		// Draw a point for the node
		if(node.current)
			glUniform4f(loc, 1.0f, 0.f, 0.f, 1.f);
		else if(node.open)
			glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
		else if(node.closed)
			glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
		else
			continue;
			//glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
		rManager.M = glm::translate(glm::mat4(1.0f), node.position);
		rManager.markMDirty();
		rManager.setShaderMatricies(*shader);
		BasicShapes::drawPoint(4,vploc);

		// Draw all node links
		rManager.M = glm::mat4(1.0f);
		rManager.markMDirty();
		rManager.setShaderMatricies(*shader);
		for(std::vector<struct PathNodeLink *>::iterator j = node.links.begin(); j != node.links.end(); j++)
		{
			struct PathNodeLink *nodeLnk = *j;
			if(nodeLnk->a->current && nodeLnk->b->current)
				glUniform4f(loc, 1.0f, 0.f, 0.f, 1.f);
			else if((nodeLnk->a->closed && (nodeLnk->b->current || nodeLnk->b->closed)) || (nodeLnk->b->closed && (nodeLnk->a->current || nodeLnk->a->closed)))
				glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
			else if((nodeLnk->a->open && nodeLnk->b->closed) || (nodeLnk->b->open && nodeLnk->a->closed))
				glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
			else if(
				((nodeLnk->a->current || nodeLnk->a->open || nodeLnk->a->closed) && (!nodeLnk->b->current && !nodeLnk->b->open && !nodeLnk->b->closed)) || 
				((nodeLnk->b->current || nodeLnk->b->open || nodeLnk->b->closed) && (!nodeLnk->a->current && !nodeLnk->a->open && !nodeLnk->a->closed))
			       )
				glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
			else
				continue;
			//{
			//	if(nodeLnk->a->closed && nodeLnk->b->closed)
			//		glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
			//	else if(nodeLnk->a->open && nodeLnk->b->open)
			//		glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
			//	else
			//		glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);
			//}
			BasicShapes::drawLine(nodeLnk->a->position, nodeLnk->b->position, vploc);
		}
	}
	glDisable(GL_BLEND);
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
