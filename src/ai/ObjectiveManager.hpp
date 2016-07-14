#ifndef __OBJECTIVEMANAGER_H_INCLUDED__
#define __OBJECTIVEMANAGER_H_INCLUDED__

#define OBJECTIVE_WAITING 0
#define OBJECTIVE_LOCATE_ARTEFACT 1
#define OBJECTIVE_COLLECT_ATREFACT 2
#define OBJECTIVE_EXIT_MUSEUM 3

#define OBJECTIVE_ATTACK_PLAYER 4
#define OBJECTIVE_ATTACK_TURRET 5

#define OBJECTIVE_WAITING_WAIT_TIME 2
#define OBJECTIVE_COLLECT_ARTEFACT_WAIT_TIME 4

#include "ai/path/PathExecuter.hpp"
#include "ai/path/PathFinder.hpp"
#include "world/entities/Enemy.hpp"
#include "util/DeltaTime.hpp"

namespace ai {
	class ObjectiveManager {
		private:
			int currentObjective;
			double artefactCollectionStartTime;
			PathNode *exitNode,*artefactNode;
			world::World *world;
			world::entities::Enemy *enemy;
			ai::path::PathExecuter *pathExecuter;
			ai::path::PathFinder *pathFinder;
			ai::path::NavigationGraph *navigationGraph;
			PathNode *targetNode;
			float targetRadius;
		public:
			ObjectiveManager(world::World *world, world::entities::Enemy *enemy, ai::path::PathExecuter *pathExecuter, ai::path::NavigationGraph *navigationGraph);
			~ObjectiveManager();
			void changeState(int i);
			void tick(util::DeltaTime &deltaTime);
			void render(render::RenderManager &rManager);
	};
}

#endif
