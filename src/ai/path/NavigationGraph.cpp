#include "util/gl.h"
#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include <set>
#include "util/StreamUtils.hpp"

#include "NavigationGraph.hpp"

using namespace ai::path;
using namespace util::StreamUtils;
using namespace render;

NavigationGraph::NavigationGraph(int assetId, std::istream &fp) : Asset(assetId)
{
	setName(readString(fp));
	this->numNodes = readInt(fp);
	this->nodes = new PathNode*[this->numNodes];
	for(int i=0;i<this->numNodes;i++)
	{
		PathNode *(&n) = this->nodes[i];
		n = new PathNode;
		n->id = i;
		n->position.x = readFloat(fp);
		n->position.y = readFloat(fp);
		n->position.z = readFloat(fp);
	}
	// Create node links
	int numNodeLinks = readInt(fp);
	for(int i=0;i<numNodeLinks;i++)
	{
		PathNodeLink *pnLink = new PathNodeLink;
		pnLink->a = this->nodes[readInt(fp)-1];
		pnLink->b = this->nodes[readInt(fp)-1];
		pnLink->dist = glm::distance(pnLink->a->position, pnLink->b->position);
		pnLink->a->links.push_back(pnLink);
		pnLink->b->links.push_back(pnLink);
	}
}
NavigationGraph::~NavigationGraph()
{
	std::set<struct PathNodeLink *> links;
	for(int i=0;i<this->numNodes;i++)
	{
		auto &n = this->nodes[i];
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
}
void NavigationGraph::render(render::RenderManager &rManager)
{
	glEnable(GL_BLEND);
	shaders::ShaderProgram *shader = shaders::ShaderProgram::getShader(SHADER_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	GLint loc = shader->getShaderLocation(false, SHADER_solidColor_solidColor);
	for(int i=0;i<this->numNodes;i++)
	//for(std::vector<struct PathNode *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		//struct PathNode *node = *it;
		PathNode *node = this->nodes[i];
		// Draw all node links
		for(std::vector<struct PathNodeLink *>::iterator j = node->links.begin(); j != node->links.end(); j++)
		{
			struct PathNodeLink *nodeLnk = *j;
			rManager.M = glm::mat4(1.0f);
			rManager.markMDirty();
			rManager.setShaderMatricies(*shader);
			glUniform4f(loc, 0.0f, 0.f, 0.4f, 1.f);
			BasicShapes::drawLine(nodeLnk->a->position, nodeLnk->b->position, vploc);
		}
		// Draw a point for the node
		rManager.useShader(SHADER_solidColor);
		rManager.M = glm::translate(glm::mat4(1.0f), node->position);
		rManager.markMDirty();
		rManager.setShaderMatricies(*shader);
		if(node->current)
			glUniform4f(loc, 1.0f, 0.f, 0.f, 1.f);
		else if(node->closed)
			glUniform4f(loc, 1.0f, 1.f, 0.f, 1.f);
		else if(node->open)
			glUniform4f(loc, 0.0f, 1.f, 0.f, 1.f);
		else
			glUniform4f(loc, 0.0f, 0.f, 1.f, 1.f);

		BasicShapes::drawPoint(3,vploc);
		
	}
	glDisable(GL_BLEND);
}
