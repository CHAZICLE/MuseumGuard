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

	this->perception = new PerceptionManager(pEnemy, {&typeid(Turret), &typeid(Player)}, glm::vec3());
	this->aimBot = new AimBot(this->perception);

	this->entranceNode = 0;
	this->artefactCollectionStartTime = 0.f;
	this->targetNode = 0;
	this->targetRadius = 1.f;
	this->collectedArtefact = false;
}
ObjectiveManager::~ObjectiveManager()
{
	delete this->pathExecuter;
	delete this->pathFinder;
	delete this->pathExplorer;
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
			{
				// Select an artefact
				this->artefactNode = this->navigationGraph->getRandomNode(ASSET_WORLD_NAV_OBJ_GROUP_ARTEFACT);
				this->entranceNode = this->navigationGraph->getRandomNode(ASSET_WORLD_NAV_OBJ_GROUP_EXIT);
				this->enemy->setPosition(this->entranceNode->position);
				this->targetNode = 0;
				this->visitedExitNodes.clear();
				this->visitedExitNodes.insert(this->entranceNode->id);
				this->currentObjective = OBJECTIVE_LOCATE_ARTEFACT;
			}
			return;
		case OBJECTIVE_LOCATE_ARTEFACT:
			// If we've found the artefact
			if(glm::distance2(this->enemy->getPosition(), this->artefactNode->position)<=2*2)
			{
				this->artefactCollectionStartTime = deltaTime.getTime();
				this->currentObjective = OBJECTIVE_COLLECT_ATREFACT;
			}
			// If we reach the latest portal node
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
			this->perceptionCheck(deltaTime);
			return;
		case OBJECTIVE_COLLECT_ATREFACT:
			// If we've been collecting the artefact long enough
			if(deltaTime.getTime()-this->artefactCollectionStartTime>=OBJECTIVE_COLLECT_ARTEFACT_WAIT_TIME)
			{
				currentNode = this->navigationGraph->getNearestPathNodeFromPool(this->enemy->getPosition(), this->visitedExitNodes);
				this->targetNode = this->navigationGraph->getNearestPathNode(currentNode->position, 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				this->pathFinder->start(currentNode, this->targetNode);
				this->collectedArtefact = true;
				this->currentObjective = OBJECTIVE_EXIT_MUSEUM;
			}
			return;
		case OBJECTIVE_EXIT_MUSEUM:
			// If we've reached the exit
			if(this->targetNode==0 || glm::distance2(this->enemy->getPosition(), this->targetNode->position)<=this->targetRadius*this->targetRadius)
			{
				this->world->gameOver(GAME_OVER_ENEMY_ESCAPED_W_ARTEFACT);
			}
			this->perceptionCheck(deltaTime);
			return;
		case OBJECTIVE_ATTACK_TURRET: // Attack turret

			return;
		case OBJECTIVE_ATTACK_PLAYER: // Attack player
			if(this->targetNode!=0)
			{
				float targetDistanceToLastKnown = glm::distance(this->targetNode->position*glm::vec3(1,1,0), this->lastKnownPlayerPosition*glm::vec3(1,1,0));
				float targetDistanceToEnemy = glm::distance(this->targetNode->position*glm::vec3(1,1,0), this->enemy->getPosition()*glm::vec3(1,1,0));
				if(targetDistanceToEnemy<=2.f || targetDistanceToLastKnown>=2.f)// If player moved
				{
					this->targetNode = 0;
				}
			}
			if(this->targetNode==0)
			{
				currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				this->targetNode = this->navigationGraph->getNearestPathNode(this->lastKnownPlayerPosition, 1<<ASSET_WORLD_NAV_OBJ_GROUP_HIGHDETAIL);
				float targetDistanceToEnemy = glm::distance(this->targetNode->position*glm::vec3(1,1,0), this->enemy->getPosition()*glm::vec3(1,1,0));
				if(targetDistanceToEnemy<=2.f)
				{
					this->targetNode = 0;
					if(this->collectedArtefact)
						this->currentObjective = OBJECTIVE_EXIT_MUSEUM;
					else
						this->currentObjective = OBJECTIVE_LOCATE_ARTEFACT;
					return;
				}
				if(currentNode!=this->targetNode)
					this->pathFinder->start(currentNode, this->targetNode);
			}
			this->perceptionCheck(deltaTime);
			return;
	}
}
void ObjectiveManager::perceptionCheck(util::DeltaTime &deltaTime)
{
	this->perception->tick(deltaTime);
	this->aimBot->tick(deltaTime);
	Entity *perc = this->perception->getPerceivedEntity();
	if(dynamic_cast<Turret *>(perc)!=0)
	{
		if(this->currentObjective!=OBJECTIVE_ATTACK_TURRET)
			this->targetNode = 0;
		this->targetTurret = (Turret *)this->perception->getPerceivedEntity();
		this->currentObjective = OBJECTIVE_ATTACK_TURRET;
	}
	if(dynamic_cast<Player *>(perc)!=0)
	{
		if(this->currentObjective!=OBJECTIVE_ATTACK_PLAYER)
			this->targetNode = 0;
		this->lastKnownPlayerPosition = this->perception->getPerceivedEntity()->getPosition();
		this->currentObjective = OBJECTIVE_ATTACK_PLAYER;
	}
	Turret *t = dynamic_cast<Turret *>(perc);
	if(t!=0)
	{
		this->currentObjective = OBJECTIVE_ATTACK_TURRET;
		this->knownTurrets.insert(t);
	}
}
void ObjectiveManager::render(render::RenderManager &rManager)
{
	this->aimBot->render(rManager);
}
