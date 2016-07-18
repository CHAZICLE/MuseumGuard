#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/gl.h"
#include "util/DeltaTime.hpp"
#include "util/AssetManager.hpp"
#include "util/Boundaries.hpp"
#include "util/QuaternionUtils.hpp"

#include "render/BasicShapes.hpp"
#include "render/shaders/ShaderUtils.hpp"
#include "render/RenderManager.hpp"
#include "render/StaticModel.hpp"
#include "render/SkeletalModel.hpp"
#include "render/SkeletalAnimation.hpp"

#include "world/entities/Enemy.hpp"
#include "world/entities/Player.hpp"
#include "world/entities/SecurityCamera.hpp"
#include "world/entities/Turret.hpp"

#include "input/controls/PlayerGameControls.hpp"

//debug
#include "input/controls/DebugControls.hpp"
#include "world/collisions/StaticMesh.hpp"
#include <glm/gtx/quaternion.hpp>
#include "ai/path/PathFinder.hpp"
//debug end

#include "World.hpp"

using namespace util;
using namespace world;
using namespace entities;
using namespace render;
using namespace util::Boundaries;
using namespace ai::path;

AABB aabb;

//debug
float distance = 5.f;
float size = 0.5f;
bool debug_renderEntityMarkers = false;
bool debug_renderEntityBounds = false;
int debug_renderWireframe = 0;
bool debug_renderNavGraph = false;
PathNode *a=0,*b=0;
PathFinder *f = new PathFinder();
glm::vec3 debug_point;
glm::vec3 debug_point2,debug_point3;
//dbend


World::World()
{
	this->world_interactive_model = (render::StaticModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_WORLD_INTERACTIVE_OBJ);
	this->world_aesthetic_model = 0;
	this->world_skybox = (render::StaticModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SKYBOX_OBJ);
	this->world_interactive_collision = new collisions::StaticMesh();
	this->world_navigation_graph = (NavigationGraph *)util::AssetManager::getAssetManager()->getAsset(ASSET_WORLD_NAV_OBJ);

	this->selector = SELECTOR_OFF;
	this->enableSelector = true;
	this->selectorCamera = new SecurityCamera();
	this->selectorTurret = new Turret();

	this->entities.clear();
	this->spawningEntities.clear();
	
	this->player = new Player();
	this->enemy = new Enemy(this);
	
	this->controlScheme = new controls::PlayerGameControls((Entity *)this->player, this->world_interactive_collision);
	
	this->vertAngle = 0;
	this->horizAngle = 0;
	this->lastX = 0;
	this->lastY = 0;
	this->selectorYaw = 0.f;
	
	this->world_interactive_collision->addStaticModel(this->world_interactive_model);
	
	
	this->selectorCamera->selector = true;
	this->selectorTurret->selector = true;
	
	this->add(this->selectorCamera, glm::vec3(20,0,5));
	this->add(this->selectorTurret, glm::vec3(20,0,5));
	this->add(this->player, glm::vec3(0,-20,2));
	this->add(this->enemy, glm::vec3(0,-10,0));
	
	this->remainingTurrets = 3;
	this->remainingCameras = 5;

	std::ifstream wf("./player.dat");
	if(wf.is_open())
	{
		glm::vec3 a;
		glm::quat q;
		wf.read((char *)&a, sizeof(glm::vec3));
		wf.read((char *)&q, sizeof(glm::quat));
		wf.close();
		this->player->setPosition(a);
		this->player->setOrientation(q);
	}
}
World::~World()
{
	std::ofstream wf("./player.dat");
	glm::vec3 p = this->player->getPosition();
	wf.write((char *)&p, sizeof(glm::vec3));
	glm::quat q = this->player->getOrientation();
	wf.write((char *)&q, sizeof(glm::quat));
	wf.close();
	delete this->world_interactive_collision;
	delete this->controlScheme;
	for(auto ent : this->spawningEntities)
		delete ent;
	for(auto ent : this->entities)
		delete ent;
}
void World::add(Entity *ent, glm::vec3 location)
{
	ent->setPosition(location);
	this->spawningEntities.push_back(ent);
	//ent->world = this;
	//this->entities.push_back(ent);
}
util::DeltaTime *debug_deltaTime;
void World::tick(util::DeltaTime &deltaTime, bool surface)
{
	debug_deltaTime = &deltaTime;
	if(surface)
	{
		//this->enemy->setPosition(glm::vec3(std::sin(deltaTime.getTime()/2)*10, 3, 0));
		//this->camera->setPosition(glm::vec3(std::sin(deltaTime.getTime())*4, std::cos(deltaTime.getTime())*4, std::sin(deltaTime.getTime()/2+M_PI/2)*5+10));
		//this->camera->setOrientation(glm::quat(glm::vec3(deltaTime.getTime(), 0, 0)));
		//this->controlScheme->tick(deltaTime);
		for(Entity *ent : this->entities)
		{
			ent->tick(deltaTime);
		}
		Entity *ent;
		for(std::list<Entity *>::iterator it = this->spawningEntities.begin(); it!=this->spawningEntities.end();)
		{
			ent = *it;
			it = this->spawningEntities.erase(it);
			ent->addedToWorld(this, deltaTime.getTime());
			this->entities.push_back(ent);
		}
		((controls::PlayerGameControls *)this->controlScheme)->tick2(0, *debug_deltaTime);
		this->viewDirection = this->player->getOrientation()*glm::vec3( 0, 1, 0);
		this->viewUp = this->player->getOrientation()*glm::vec3(0, 0, 1);
	}
	if(this->enableSelector)
	{
		Raycast raycast;
		raycast.origin = this->player->getPosition();
		raycast.direction = this->viewDirection;
		raycast.maxDistance = 8;
		RaycastResult result = this->world_interactive_collision->rayCast(raycast);
		this->selectorRemove = 0;

		//for(auto e : this->entities)
		//{
		//	if(e!=this->selectorTurret && dynamic_cast<Turret *>(e)!=0)
		//	{
		//		AABB aabb = e->getBounds()->translate(e->getPosition());
		//		if(aabb.rayCastCheck(raycast))
		//		{
		//			this->selector = SELECTOR_REMOVE;
		//			this->selectorRemove = e;
		//			this->selectorCamera->doRender = false;
		//			this->selectorTurret->doRender = false;
		//		}
		//	}
		//	if(e!=this->selectorCamera && dynamic_cast<SecurityCamera *>(e)!=0)
		//	{
		//		AABB aabb = e->getBounds()->translate(e->getPosition());
		//		if(aabb.rayCastCheck(raycast))
		//		{
		//			this->selector = SELECTOR_REMOVE;
		//			this->selectorRemove = e;
		//			this->selectorCamera->doRender = false;
		//			this->selectorTurret->doRender = false;
		//		}
		//	}
		//}
		if(result.hit)
		{
			raycast.maxDistance = result.distance;
			glm::vec3 rayHit = this->player->getPosition()+(this->viewDirection*result.distance);
			//std::cout << "RAY: " << result.distance << "hitNormal=(" << result.hitNormal.x << ", " << result.hitNormal.y << ", " << result.hitNormal.z << ") " << std::endl;
			double pitch = -std::atan2(std::sqrt(result.hitNormal.x*result.hitNormal.x+result.hitNormal.y*result.hitNormal.y), result.hitNormal.z)+glm::radians(90.f);
			AABB aabb;
			if(pitch>=M_PI/2-0.01f)
			{
				this->selector = SELECTOR_TURRET;
				this->selectorCamera->doRender = false;
				this->selectorTurret->doRender = true;
				this->selectorTurret->setPosition(rayHit);
				aabb = this->selectorTurret->getBounds()->translate(this->selectorTurret->getPosition());
			}
			else
			{
				this->selector = SELECTOR_SECURITY_CAMERA;
				this->selectorCamera->doRender = true;
				this->selectorTurret->doRender = false;
				if(pitch>=-M_PI/2+0.01f)
					this->selectorYaw = -std::atan2(result.hitNormal.x, result.hitNormal.y);
				this->selectorCamera->setOrientation(glm::quat(glm::vec3(pitch, 0, this->selectorYaw)));
				this->selectorCamera->setPosition(rayHit);
				aabb = this->selectorCamera->getBounds()->translate(this->selectorCamera->getPosition());
			}
			AABB *_eaabb,eaabb;
			for(auto e : this->entities)
			{
				if(e==this->selectorTurret || e==this->selectorCamera)
					continue;
				_eaabb = e->getBounds();
				if(_eaabb==0)
					continue;
				eaabb = _eaabb->translate(e->getPosition());
				if(dynamic_cast<Turret *>(e)!=0 || dynamic_cast<SecurityCamera *>(e)!=0)
				{
					if(eaabb.rayCastCheck(raycast) || eaabb.checkIntersect(aabb))
					{
						this->selector = SELECTOR_REMOVE;
						this->selectorCamera->doRender = false;
						this->selectorTurret->doRender = false;
						this->selectorRemove = e;
					}
				}
				else if((e!=this->player && eaabb.rayCastCheck(raycast)) || eaabb.checkIntersect(aabb))
				{
					this->selector = SELECTOR_OFF;
					this->selectorCamera->doRender = false;
					this->selectorTurret->doRender = false;
				}
			}
		}
		else
		{
			this->selector = SELECTOR_OFF;
			this->selectorCamera->doRender = false;
			this->selectorTurret->doRender = false;
		}
	}
	///////////////////
	// DEBUG TICKING //
	///////////////////
	
	Raycast raycast;
	raycast.origin = this->player->getPosition();
	raycast.direction = this->viewDirection;
	raycast.maxDistance = 0;
	RaycastResult result = this->world_interactive_collision->rayCast(raycast),*_result;
	if(result.hit)
		raycast.maxDistance = result.distance;
	AABB *_eaabb,eaabb;
	for(auto e : this->entities)
	{
		if(e==this->selectorTurret || e==this->selectorCamera || e==this->player)
			continue;
		_eaabb = e->getBounds();
		if(_eaabb==0)
			continue;
		eaabb = _eaabb->translate(e->getPosition());
		_result = eaabb.rayCast(raycast);
		if(_result!=0)
		{
			if(_result->distance<result.distance)
				result = *_result;
			delete _result;
		}
	}
	
	if(result.hit)
	{
		glm::vec3 boxCent = this->player->getPosition()+this->viewDirection*result.distance;
		debug_point = boxCent;
		aabb.boxCenter[0] = boxCent.x;
		aabb.boxCenter[1] = boxCent.y;
		aabb.boxCenter[2] = boxCent.z;
		aabb.boxHalfSize[0] = size/2;
		aabb.boxHalfSize[1] = size/2;
		aabb.boxHalfSize[2] = size/2;
	}
}
double d = 0;
void World::render(render::RenderManager &rManager, bool isSurface)
{
	// Setup rendering
	rManager.enableCullFace();
	
	// Setup skybox view matrix
	rManager.V = glm::lookAt(
			glm::vec3(0,0,1.f),
			glm::vec3(0,0,1.f)+viewDirection,
			viewUp
		);
	rManager.M = glm::mat4(1.0f), glm::vec3(1,1,1);
	rManager.markVDirty();
	rManager.markMDirty();
	glCullFace(GL_FRONT);
	this->world_skybox->render(rManager, SHADER_UVTest);
	glCullFace(GL_BACK);
	
	// Setup view matrix
	rManager.V = glm::lookAt(
			this->player->getPosition(),
			this->player->getPosition() + viewDirection,
			viewUp
		);
	rManager.markVDirty();

	rManager.enableDepth();

	
	// Render the world model
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	this->world_interactive_model->render(rManager, SHADER_UVTest);

	for(Entity *ent : this->entities)
	{
		//if(this->selector!=SELECTOR_SECURITY_CAMERA && this->selectorCamera==ent)
		//	continue;
		//if(this->selector!=SELECTOR_TURRET && this->selectorTurret==ent)
		//	continue;
		//if(ent==this->player)
		//	continue;
		if(ent->doRender)
			ent->render(rManager);
	}
	
	/////////////////////
	// DEBUG RENDERING //
	/////////////////////

	rManager.disableDepth();
	rManager.disableCullFace();



	if(debug_renderEntityMarkers || debug_renderEntityBounds)
	{
		for(auto ent : this->entities)
		{
			if(ent->doRender)
				ent->renderDebug(rManager, debug_renderEntityMarkers, debug_renderEntityBounds);
		}
	}
	if(debug_renderEntityMarkers)
	{
		// Draw world origin marker
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3(10, 0, 0), glm::vec4(1.f, 0.f, 0.f, 1.f));
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3( 0,10, 0), glm::vec4(0.f, 1.f, 0.f, 1.f));
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3( 0, 0,10), glm::vec4(0.f, 0.f, 1.f, 1.f));
		//rManager.renderOrientation(this->player->getPosition(), this->player->getOrientation());
		//rManager.renderDirectionVector(debug_point, debug_point2-debug_point, glm::vec4(1.0f, 0.f, 0.f, 1.0f));
		rManager.renderDirectionVector(glm::vec3(), debug_point, glm::vec4(1.0f, 0.f, 0.f, 1.0f));
		rManager.renderDirectionVector(glm::vec3(), debug_point2, glm::vec4(0.0f, 1.f, 0.f, 1.0f));
		rManager.renderDirectionVector(glm::vec3(), debug_point3, glm::vec4(0.0f, 0.f, 1.f, 1.0f));
	}
	// Render bounds of entities
	if(debug_renderEntityBounds)
	{
		aabb.render(rManager, glm::vec4(1.0f, 0.0f, 0.0f, 0.8f), false);
	}
	// Wireframe
	if(debug_renderWireframe==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(debug_renderWireframe==2)
	{
		rManager.M = glm::mat4(1.0f);
		rManager.markMDirty();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->world_interactive_model->render(rManager, SHADER_UVTest);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// Navgraph
	if(debug_renderNavGraph)
		this->world_navigation_graph->render(rManager);

	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_rightVector, glm::vec4(0.f,1.f,1.f,1.f));
	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_relUpVector, glm::vec4(1.f,0.f,1.f,1.f));
	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_hitNormal, glm::vec4(1.f,1.f,0.f,1.f));

	//if(isSurface)
		//((controls::PlayerGameControls *)this->controlScheme)->tick2(rManager, *debug_deltaTime);


	//glm::vec3 boxCent;

	//world_model->getBounds().render(rManager, glm::vec4(0.f, 0.f, 1.f, 1.f), false);
	//sm->render(rManager, raycast);
	//sm->render(rManager, aabb);
	double j = glfwGetTime();
	if(f!=0 && !f->done)// && j>d)
	{
		d = j;
		if(f->tick(5))
		{
			PRINT_DEBUG("DONE");
			if(f->sucess)
				this->enemy->setPath(f->getPath());
		}
	}
	else
	{
		/*
		for(int i=0;i<this->world_navigation_graph->numNodes;i++)
		{
			PathNode *n = &this->world_navigation_graph->nodes[i];
			if(aabb.checkInside(n->position))
				n->current = true;
			else
				n->current = false;
		}
		*/
	}
}
std::list<world::Entity *> *World::getEntities()
{
	return &this->entities;
}
util::Boundaries::RaycastResult World::rayCast(util::Boundaries::Raycast &raycast, world::Entity **entity)
{
	Entity *skipEnt = *entity;
	*entity = 0;
	RaycastResult result = this->world_interactive_collision->rayCast(raycast);
	RaycastResult *_result;
	if(result.hit)
		raycast.maxDistance = result.distance;
	AABB *_eaabb,eaabb;
	for(auto e : this->entities)
	{
		if(e==this->selectorTurret || e==this->selectorCamera || e==skipEnt)
			continue;
		_eaabb = e->getBounds();
		if(_eaabb==0)
			continue;
		eaabb = _eaabb->translate(e->getPosition());
		_result = eaabb.rayCast(raycast);
		if(_result!=0)
		{
			if(_result->distance<result.distance)
			{
				*entity = e;
				result = *_result;
			}
			delete _result;
		}
	}
	return result;
}
void World::onDebugControl(Control control, int action)
{
	if(action!=CONTROL_KEYACTION_REPEAT)
		PRINT_CONTROL("A", control, action);

	// Debug config
	if(action==CONTROL_KEYACTION_RELEASE)
	{
		if(control==GLFW_KEY_F5) debug_renderEntityMarkers = !debug_renderEntityMarkers;
		if(control==GLFW_KEY_F6) debug_renderEntityBounds = !debug_renderEntityBounds;
		if(control==GLFW_KEY_F7) debug_renderWireframe++;
		if(control==GLFW_KEY_F8) debug_renderNavGraph = !debug_renderNavGraph;
		if(control==GLFW_KEY_T)
		{
			this->player->setPosition(glm::vec3(0,-20,2));
			this->player->setOrientation(glm::quat());
		}
		if(control==GLFW_KEY_Y)
		{
			this->player->setPosition(glm::vec3(0,-20,20));
			this->player->setOrientation(glm::quat());
		}
		if(debug_renderWireframe>2)
			debug_renderWireframe = 0;
		if(control==GLFW_KEY_O)
		{
			for(int i=0;i<this->world_navigation_graph->numNodes;i++)
			{
				PathNode *n = &this->world_navigation_graph->nodes[i];
				if(aabb.checkInside(n->position))
				{
					PRINT_DEBUG("node:" << i);
					n->current = true;
					if(a==0)
						a = n;
					else
						b = n;
				}
			}
		}
//13,84
		if(control==GLFW_KEY_I)
		{
			PRINT_DEBUG("Setting default");
			a = &this->world_navigation_graph->nodes[0];
			b = &this->world_navigation_graph->nodes[1];
			a->current = true;
			b->current = true;
		}
		if(control==GLFW_KEY_P)
		{
			PRINT_DEBUG("Clearing navgraph");
			a = 0;
			b = 0;
			for(int i=0;i<this->world_navigation_graph->numNodes;i++)
			{
				PathNode *n = &this->world_navigation_graph->nodes[i];
				n->current = false;
				n->closed = false;
				n->open = false;
			}
		}
		if(control==GLFW_KEY_LEFT_BRACKET)
		{
			PRINT_DEBUG("Running path");
			if(a!=0 && b!=0)
			{
				a->current = false;
				b->current = false;
				f->start(a,b);
				PRINT_DEBUG("START");
			}
		}
	}

	if(control==GLFW_MOUSE_BUTTON_2 && action==CONTROL_MOUSEBUTTONACTION_RELEASE)
	{
		this->enableSelector = true;
	}
	if(action==CONTROL_MOUSEBUTTONACTION_PRESS)
	{
		if(this->selector==SELECTOR_SECURITY_CAMERA)
		{
			SecurityCamera *cam = new SecurityCamera();
			cam->setOrientation(this->selectorCamera->getOrientation());
			this->add(cam, this->selectorCamera->getPosition());
			this->enableSelector = false;
			this->selector = SELECTOR_OFF;
			this->selectorCamera->doRender = false;
			this->selectorTurret->doRender = false;
		}
		else if(this->selector==SELECTOR_TURRET)
		{
			Turret *turret = new Turret();
			turret->setOrientation(this->selectorTurret->getOrientation());
			this->add(turret, this->selectorTurret->getPosition());
			this->enableSelector = false;
			this->selector = SELECTOR_OFF;
			this->selectorCamera->doRender = false;
			this->selectorTurret->doRender = false;
		}
		else if(this->selector==SELECTOR_REMOVE)
		{
			this->entities.remove(this->selectorRemove);
			delete this->selectorRemove;
			this->selectorRemove = 0;
			this->selectorCamera->doRender = false;
			this->selectorTurret->doRender = false;
		}
	}
}
void World::onRayHit(glm::vec3 rayOrigin, float distance, glm::vec3 normal)
{
	
}
