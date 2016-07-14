#include <glm/gtx/norm.hpp>
#include "obj/world.nav.obj.h"

#include "ObjectiveManager.hpp"

using namespace ai;
using namespace ai::path;
using namespace world;
using namespace world::entities;

ObjectiveManager::ObjectiveManager(World *world, Enemy *enemy, PathExecuter *pathExecuter, NavigationGraph *navigationGraph)
{
	this->world = world;
	this->enemy = enemy;
	this->pathExecuter = pathExecuter;
	this->currentObjective = OBJECTIVE_WAITING;
	this->pathFinder = new PathFinder();
	this->navigationGraph = navigationGraph;

	this->artefactCollectionStartTime = 0.f;
	this->targetNode = 0;
	this->targetRadius = 1.f;
}
ObjectiveManager::~ObjectiveManager()
{
	
}
void ObjectiveManager::changeState(int i)
{
	PRINT_DEBUG("New state:" << i);
	PathNode *currentNode;
	switch(i)
	{
		case OBJECTIVE_LOCATE_ARTEFACT:
			this->exitNode = this->navigationGraph->getRandomNode(world_nav_obj_exit);
			this->artefactNode = this->navigationGraph->getRandomNode(world_nav_obj_artifacts);
			this->enemy->setPosition(exitNode->position);

			currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<world_nav_obj_rooms);
			this->targetNode = this->navigationGraph->getNearestPathNode(artefactNode->position, 1<<world_nav_obj_rooms);
			this->pathFinder->start(currentNode, this->targetNode);
			break;
		case OBJECTIVE_EXIT_MUSEUM:
			currentNode = this->navigationGraph->getNearestPathNode(this->enemy->getPosition(), 1<<world_nav_obj_rooms);
			this->targetNode = this->navigationGraph->getNearestPathNode(exitNode->position, 1<<world_nav_obj_rooms);
			this->pathFinder->start(currentNode, this->targetNode);
			break;
	}
	this->currentObjective = i;
}
void ObjectiveManager::tick(util::DeltaTime &deltaTime)
{
	if(!this->pathFinder->done)
	{
		if(this->pathFinder->tick(20) && this->pathFinder->sucess)
		{
			PRINT_DEBUG("PATH COMPLETE");
			this->enemy->setPath(this->pathFinder->getPath());
		}
	}
	switch(this->currentObjective)
	{
		case OBJECTIVE_WAITING: // TODO: Delay
			if(deltaTime.getTime()-this->enemy->getSpawnTime()>=OBJECTIVE_WAITING_WAIT_TIME)
				this->changeState(OBJECTIVE_LOCATE_ARTEFACT);
			return;
		case OBJECTIVE_LOCATE_ARTEFACT: // Locate the exhibit
			if(this->targetNode==0 || glm::distance2(this->enemy->getPosition(), this->artefactNode->position)<=this->targetRadius*this->targetRadius)
			{
				PRINT_DEBUG("FOUND ARTEFACT");
				this->artefactCollectionStartTime = deltaTime.getTime();
				this->changeState(OBJECTIVE_COLLECT_ATREFACT);
			}
			return;
		case OBJECTIVE_COLLECT_ATREFACT: // Sit near exhibit and collect, delay
			if(deltaTime.getTime()-this->artefactCollectionStartTime>=OBJECTIVE_COLLECT_ARTEFACT_WAIT_TIME)
			{
				PRINT_DEBUG("ARTEFACT COLLECTED");
				this->changeState(OBJECTIVE_EXIT_MUSEUM);
			}
			return;
		case OBJECTIVE_EXIT_MUSEUM: // Leave the museum
			if(this->targetNode==0 || glm::distance2(this->enemy->getPosition(), this->exitNode->position)<=this->targetRadius*this->targetRadius)
			{
				PRINT_DEBUG("EXITED MUSEUM");
			}
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
