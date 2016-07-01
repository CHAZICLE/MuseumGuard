#include "util/gl.h"
#include "render/shaders/ShaderUtils.hpp"
#include "render/BasicShapes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include <set>

#include "NavigationGraph.hpp"

using namespace render;

inline struct PathNode *getNode(NavigationGraph *h, int x, int y)
{
	unsigned long i = x*50+y;
	if(x>=50 || y>=50 || i>=h->nodes.size())
		return 0;
	return h->nodes.at(i);
}
inline void linkNodes(struct PathNode *a, struct PathNode *b)
{
	if(a==0 || b==0)
		return;
	struct PathNodeLink *pnLink = new struct PathNodeLink;
	pnLink->a = a;
	pnLink->b = b;
	pnLink->dist = glm::distance(a->position, b->position);
	a->links.push_back(pnLink);
	b->links.push_back(pnLink);
}

NavigationGraph::NavigationGraph()
{
	int id = 0;
	// Create node grid
	for(int x=0;x<50;x++)
	{
		for(int y=0;y<50;y++)
		{
			struct PathNode *node = new struct PathNode;
			node->id = ++id;
			node->position = glm::vec3(x*5, y*5, 0);
			this->nodes.push_back(node);
		}
	}
	// Create node links
	for(int x=0;x<50;x++)
	{
		for(int y=0;y<50;y++)
		{
			if(x>20 && x<30 && y>20 && y<30)
				continue;
			struct PathNode *a,*b;
			a = getNode(this, x, y);
			b = getNode(this, x+1, y);
			linkNodes(a,b);
			
			b = getNode(this, x, y+1);
			linkNodes(a,b);
		}
	}
}
NavigationGraph::~NavigationGraph()
{
	std::set<struct PathNodeLink *> links;
	for(auto &n : this->nodes)
	{
		for(auto &nLink : n->links)
			links.insert(nLink);
	}
	for(auto &n : links)
		delete n;
	for(auto &n : this->nodes)
	{
		delete n;
	}
}
void NavigationGraph::render(util::DeltaTime &deltaTime, render::RenderManager &rManager)
{
	glEnable(GL_BLEND);
	shaders::ShaderProgram *shader = shaders::ShaderProgram::getShader(SHADER_solidColor);
	GLint vploc = shader->getShaderLocation(false, SHADERVAR_vertex_position);
	GLint loc = shader->getShaderLocation(false, SHADER_solidColor_solidColor);
	for(std::vector<struct PathNode *>::iterator it = this->nodes.begin(); it != this->nodes.end(); it++)
	{
		struct PathNode *node = *it;
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
