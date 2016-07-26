#include <glm/gtx/norm.hpp>
#include "res/obj/world.nav.obj.h"
#include "world/World.hpp"

#include "ObjectiveManager.hpp"

using namespace ai;
using namespace ai::path;
using namespace world;
using namespace world::entities;
using namespace util;

ObjectiveManager::ObjectiveManager(World *pWorld, Enemy *pEnemy, NavigationGraph *pNavigationGraph)
{
	this->world = pWorld;
	this->enemy = pEnemy;
	this->pathExecuter = new PathExecuter(5.f, pEnemy, pNavigationGraph);
	this->currentObjective = OBJECTIVE_WAITING;
	this->pathFinder = new PathFinder();
	this->pathExplorer = new PathExplorer(this->world->world_navigation_graph);
	this->navigationGraph = pNavigationGraph;

	this->pathExplorer->setGroupWhitelist(1<<ASSET_WORLD_NAV_OBJ_GROUP_EXPLORE);
	this->pathExplorer->setGroupPOI(1<<ASSET_WORLD_NAV_OBJ_GROUP_ARTEFACT);
	this->pathExplorer->setGroupPortal(1<<ASSET_WORLD_NAV_OBJ_GROUP_PORTAL);

	this->entranceNode = 0;
	this->artefactCollectionStartTime = 0.f;
	this->targetNode = 0;
	this->targetRadius = 1.f;
}
ObjectiveManager::~ObjectiveManager()
{
	
}
void ObjectiveManager::changeState(DeltaTime &deltaTime, int i)
{
	PRINT_DEBUG("New state:" << i);
	PathNode *currentNode;
	switch(i)
	{
		case OBJECTIVE_LOCATE_ARTEFACT:
			// Select an artefact
			this->artefactNode = this->navigationGraph->getRandomNode(ASSET_WORLD_NAV_OBJ_GROUP_ARTEFACT);
			this->entranceNode = this->navigationGraph->getRandomNode(ASSET_WORLD_NAV_OBJ_GROUP_EXIT);
			this->enemy->setPosition(this->entranceNode->position);
			this->targetNode = 0;
			this->visitedExitNodes.clear();
			this->visitedExitNodes.insert(this->entranceNode->id);
			break;
		case OBJECTIVE_COLLECT_ATREFACT:
			this->artefactCollectionStartTime = deltaTime.getTime();
			break;
		case OBJECTIVE_EXIT_MUSEUM:
			currentNode = this->navigationGraph->getNearestPathNodeFromPool(this->enemy->getPosition(), this->visitedExitNodes);
			this->targetNode = this->navigationGraph->getNearestPathNode(currentNode->position, 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
			currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
			this->pathFinder->start(currentNode, this->targetNode);
			break;
	}
	this->currentObjective = i;
}
void ObjectiveManager::tick(DeltaTime &deltaTime)
{
	if(!this->pathFinder->done)
	{
		if(this->pathFinder->tick(20) && this->pathFinder->sucess)
			this->pathExecuter->postPath(this->pathFinder->getPath());
	}
	this->pathExecuter->tick(deltaTime);
	PathNode *currentNode;
	switch(this->currentObjective)
	{
		case OBJECTIVE_WAITING:
			if(deltaTime.getTime()-this->enemy->getSpawnTime()>=OBJECTIVE_WAITING_WAIT_TIME)
				this->changeState(deltaTime, OBJECTIVE_LOCATE_ARTEFACT);
			return;
		case OBJECTIVE_LOCATE_ARTEFACT: // Locate the exhibit
			if(glm::distance2(this->enemy->getPosition(), this->artefactNode->position)<=2*2)
			{
				this->changeState(deltaTime, OBJECTIVE_COLLECT_ATREFACT);
			}
			else if(this->targetNode==0 || glm::distance2(this->enemy->getPosition(), this->targetNode->position)<=this->targetRadius*this->targetRadius)
			{
				currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				this->targetNode = this->pathExplorer->getNext(this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<ASSET_WORLD_NAV_OBJ_GROUP_LOWDETAIL));
				// Find adjacent exit node
				PathNode *p;
				for(auto *l : this->targetNode->links)
				{
					p = l->getOther(this->targetNode);
					if(this->navigationGraph->getPathNodeGroupMask(p->id)&(1<<ASSET_WORLD_NAV_OBJ_GROUP_EXIT))
					{
						this->visitedExitNodes.insert(p->id);
					}
				}
				this->targetNode = this->navigationGraph->getNearestPathNode(this->targetNode->position, 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				this->pathFinder->start(currentNode, this->targetNode);
			}
			this->perceptionCheck();
			return;
		case OBJECTIVE_COLLECT_ATREFACT: // Sit near exhibit and collect, delay
			if(deltaTime.getTime()-this->artefactCollectionStartTime>=OBJECTIVE_COLLECT_ARTEFACT_WAIT_TIME)
				this->changeState(deltaTime, OBJECTIVE_EXIT_MUSEUM);
			return;
		case OBJECTIVE_EXIT_MUSEUM: // Leave the museum
			if(this->targetNode==0 || glm::distance2(this->enemy->getPosition(), this->targetNode->position)<=this->targetRadius*this->targetRadius)
			{
				PRINT_DEBUG("EXITED MUSEUM");
				//this->world->gameOver();
			}
			this->perceptionCheck();
			return;
		case OBJECTIVE_ATTACK_TURRET: // Attack turret
			return;
		case OBJECTIVE_ATTACK_PLAYER: // Attack player
			return;
	}
}
void ObjectiveManager::render(render::RenderManager &rManager)
{
}
void ObjectiveManager::perceptionCheck()
{
	
}
