#ifndef FYP_AI_OBJECTIVEMANAGER_HPP_
#define FYP_AI_OBJECTIVEMANAGER_HPP_

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
#include "ai/path/PathExplorer.hpp"
#include "ai/AimBot.hpp"
#include "world/entities/Enemy.hpp"
#include "util/DeltaTime.hpp"
#include "world/entities/Turret.hpp"
#include <set>
#include <unordered_set>

namespace ai {
	class ObjectiveManager {
		private:
			int currentObjective;
			double artefactCollectionStartTime;
			PathNode *entranceNode,*artefactNode;
			world::World *world;
			world::entities::Enemy *enemy;
			world::entities::Turret *targetTurret;
			ai::path::PathExecuter *pathExecuter;
			ai::path::PathExplorer *pathExplorer;
			ai::path::PathFinder *pathFinder;
			ai::path::NavigationGraph *navigationGraph;
			ai::PerceptionManager *perception;
			ai::AimBot *aimBot;
			glm::vec3 lastKnownPlayerPosition;
			PathNode *targetNode;
			float targetRadius;
			bool collectedArtefact;
			std::set<int> visitedExitNodes;
			std::unordered_set<world::entities::Turret *> knownTurrets;
		public:
			ObjectiveManager(world::World *world, world::entities::Enemy *enemy, ai::path::NavigationGraph *navigationGraph);
			~ObjectiveManager();
			void tick(util::DeltaTime &deltaTime);
			void render(render::RenderManager &rManager);
			void perceptionCheck(util::DeltaTime &deltaTime);
	};
}

#endif
