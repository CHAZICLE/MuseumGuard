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
PathNode *a=0,*b=0;
PathFinder *f=0;
//dbend


World::World()
{
	this->world_interactive_model = (render::StaticModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_WORLD_INTERACTIVE_OBJ);
	this->world_aesthetic_model = 0;
	this->world_skybox = (render::StaticModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SKYBOX_OBJ);
	this->world_interactive_collision = new collisions::StaticMesh();
	this->world_navigation_graph = (NavigationGraph *)util::AssetManager::getAssetManager()->getAsset(ASSET_WORLD_NAVMESH_NAV_OBJ);

	this->selector = SELECTOR_OFF;
	this->enableSelector = true;
	this->selectorCamera = new SecurityCamera();
	this->selectorTurret = new Turret();

	this->entities.clear();
	this->spawningEntities.clear();
	
	this->player = new Player();
	this->enemy = new Enemy();
	
	this->controlScheme = new controls::PlayerGameControls((Entity *)this->player, this->world_interactive_collision);
	
	this->vertAngle = 0;
	this->horizAngle = 0;
	this->lastX = 0;
	this->lastY = 0;
	
	this->world_interactive_collision->addStaticModel(this->world_interactive_model);
	
	//this->selectorCamera->keepLookingAt(this->enemy);
	//this->selectorTurret->keepLookingAt(this->enemy);
	
	this->selectorCamera->selector = true;
	this->selectorTurret->selector = true;
	
	this->add(this->selectorCamera, glm::vec3(20,0,5));
	this->add(this->selectorTurret, glm::vec3(20,0,5));
	this->add(this->player, glm::vec3(0,-20,2));
	this->add(this->enemy, glm::vec3(0,-10,0));
	
	this->remainingTurrets = 3;
	this->remainingCameras = 5;
}
World::~World()
{
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
		this->enemy->setPosition(glm::vec3(std::sin(deltaTime.getTime()/2)*10, 3, 0));
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
		this->viewDirection = this->player->getOrientation()*glm::vec3( 0, 1, 0);
		this->viewUp = this->player->getOrientation()*glm::vec3(0, 0, 1);
	}
	if(this->enableSelector)
	{
		Raycast raycast;
		raycast.origin = this->player->getPosition();
		raycast.direction = this->viewDirection;
		raycast.maxDistance = 5;
		RaycastResult result = this->world_interactive_collision->rayCast(raycast);
		this->selectorRemove = 0;

		for(auto e : this->entities)
		{
			if(e!=this->selectorTurret && dynamic_cast<Turret *>(e)!=0)
			{
				AABB *aabb = e->getBounds()->translate(e->getPosition());
				if(aabb->rayCastCheck(raycast))
				{
					this->selector = SELECTOR_REMOVE;
					this->selectorRemove = e;
				}
			}
			if(e!=this->selectorCamera && dynamic_cast<SecurityCamera *>(e)!=0)
			{
				AABB *aabb = e->getBounds()->translate(e->getPosition());
				if(aabb->rayCastCheck(raycast))
				{
					this->selector = SELECTOR_REMOVE;
					this->selectorRemove = e;
				}
			}
		}
		if(this->selectorRemove==0)
		{
			if(result.hit!=0)
			{
				glm::vec3 rayHit = this->player->getPosition()+(this->viewDirection*result.distance);
				//std::cout << "RAY: " << result.distance << "hitNormal=(" << result.hitNormal.x << ", " << result.hitNormal.y << ", " << result.hitNormal.z << ") " << std::endl;
				double pitch = -std::atan2(std::sqrt(result.hitNormal.x*result.hitNormal.x+result.hitNormal.y*result.hitNormal.y), result.hitNormal.z)+glm::radians(90.f);
				if(pitch>=M_PI/2-0.01f)
				{
					this->selectorTurret->setPosition(rayHit);
					this->selector = SELECTOR_TURRET;
				}
				else
				{
					double yaw = -std::atan2(result.hitNormal.x, result.hitNormal.y);
					this->selectorCamera->setOrientation(glm::quat(glm::vec3(pitch, 0, yaw)));
					this->selectorCamera->setPosition(rayHit);
					this->selector = SELECTOR_SECURITY_CAMERA;
				}
			}
			else
			{
				this->selector = SELECTOR_OFF;
			}
		}
	}
	///////////////////
	// DEBUG TICKING //
	///////////////////
	
	glm::vec3 boxCent = this->player->getPosition()+this->viewDirection*5.f;
	
	aabb.boxCenter[0] = boxCent.x;
	aabb.boxCenter[1] = boxCent.y;
	aabb.boxCenter[2] = boxCent.z;
	aabb.boxHalfSize[0] = size/2;
	aabb.boxHalfSize[1] = size/2;
	aabb.boxHalfSize[2] = size/2;
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
	
	// Render the world model
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	rManager.enableDepth();
	this->world_interactive_model->render(rManager, SHADER_UVTest);

	for(Entity *ent : this->entities)
	{
		if(this->selector!=SELECTOR_SECURITY_CAMERA && this->selectorCamera==ent)
			continue;
		if(this->selector!=SELECTOR_TURRET && this->selectorTurret==ent)
			continue;
		if(ent==this->player)
			continue;
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
			if(ent!=this->player)
			{
				ent->renderDebug(rManager, debug_renderEntityMarkers, debug_renderEntityBounds);
			}
		}
	}
	if(debug_renderEntityMarkers)
	{
		// Draw world origin marker
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3(10, 0, 0), glm::vec4(1.f, 0.f, 0.f, 1.f));
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3( 0,10, 0), glm::vec4(0.f, 1.f, 0.f, 1.f));
		rManager.renderDirectionVector(glm::vec3(0,0,0), glm::vec3( 0, 0,10), glm::vec4(0.f, 0.f, 1.f, 1.f));
		//rManager.renderOrientation(this->player->getPosition(), this->player->getOrientation());
	}
	if(debug_renderEntityBounds)
	{
		aabb.render(rManager, glm::vec4(1.0f, 0.0f, 0.0f, 0.8f), false);
	}

	this->world_navigation_graph->render(rManager);

	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_rightVector, glm::vec4(0.f,1.f,1.f,1.f));
	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_relUpVector, glm::vec4(1.f,0.f,1.f,1.f));
	//rManager.renderDirectionVector(this->selectorCamera->getPosition(), debug_hitNormal, glm::vec4(1.f,1.f,0.f,1.f));

	if(isSurface)
		((controls::PlayerGameControls *)this->controlScheme)->tick2(rManager, *debug_deltaTime);


	//glm::vec3 boxCent;


	//world_model->getBounds().render(rManager, glm::vec4(0.f, 0.f, 1.f, 1.f), false);
	//sm->render(rManager, raycast);
	//sm->render(rManager, aabb);
	double j = glfwGetTime();
	if(f!=0)// && j>d)
	{
		d = j;
		f->tick(5);
	}
	else
	{
		for(int i=0;i<this->world_navigation_graph->numNodes;i++)
		{
			PathNode *n = this->world_navigation_graph->nodes[i];
			if(aabb.checkInside(n->position))
				n->current = true;
			else
				n->current = false;
		}
	}
}
void World::onDebugControl(Control control, int action)
{
	if(action!=CONTROL_KEYACTION_REPEAT)
		PRINT_CONTROL("A", control, action);

	// Debug config
	if(action==CONTROL_KEYACTION_RELEASE)
	{
		if(control==GLFW_KEY_F5)
			debug_renderEntityMarkers = !debug_renderEntityMarkers;
		if(control==GLFW_KEY_F6)
			debug_renderEntityBounds = !debug_renderEntityBounds;
		if(control==GLFW_KEY_O)
		{
			for(int i=0;i<this->world_navigation_graph->numNodes;i++)
			{
				PathNode *n = this->world_navigation_graph->nodes[i];
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
			a = this->world_navigation_graph->nodes[13];
			b = this->world_navigation_graph->nodes[84];
		}
		if(control==GLFW_KEY_P)
		{
			PRINT_DEBUG("CLEAR A");
			f = 0;
			a = 0;
			b = 0;
			for(int i=0;i<this->world_navigation_graph->numNodes;i++)
			{
				PathNode *n = this->world_navigation_graph->nodes[i];
				n->current = false;
				n->closed = false;
				n->open = false;
			}
		}
		if(control==GLFW_KEY_LEFT_BRACKET)
		{
			if(a!=0 && b!=0)
			{
				if(f!=0)
					delete f;
				a->current = false;
				b->current = false;
				f = new PathFinder(a,b);
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
			cam->keepLookingAt(this->enemy);
			this->add(cam, this->selectorCamera->getPosition());
			this->enableSelector = false;
			this->selector = SELECTOR_OFF;
		}
		else if(this->selector==SELECTOR_TURRET)
		{
			Turret *turret = new Turret();
			turret->setOrientation(this->selectorTurret->getOrientation());
			turret->keepLookingAt(this->enemy);
			this->add(turret, this->selectorTurret->getPosition());
			this->enableSelector = false;
			this->selector = SELECTOR_OFF;
		}
		else if(this->selector==SELECTOR_REMOVE)
		{
			this->entities.remove(this->selectorRemove);
			delete this->selectorRemove;
			this->selectorRemove = 0;
		}
	}
}
