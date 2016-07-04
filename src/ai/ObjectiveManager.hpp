#ifndef __OBJECTIVEMANAGER_H_INCLUDED__
#define __OBJECTIVEMANAGER_H_INCLUDED__

#define OBJECTIVE_ENTER_MUSEUM 0
#define OBJECTIVE_LOCATE_EXHIBIT 1
#define OBJECTIVE_CLEAR_EXHIBIT 2
#define OBJECTIVE_COLLECT_EXHIBIT 3
#define OBJECTIVE_LEAVE_MUSEUM 4

#define OBJECTIVE_ATTACK_PLAYER 5
#define OBJECTIVE_ATTACK_TURRET 6

#include "ai/path/PathExecuter.hpp"
#include "world/Entity.hpp"

namespace ai {
	class ObjectiveManager {
		private:
			int currentObjective;
			world::World *world;
			world::Entity *entity;
			ai::path::PathExecuter *pathExecuter;
		public:
			ObjectiveManager(world::World *world, world::Entity *entity, ai::path::PathExecuter *pathExecuter);
			~ObjectiveManager();
			void tick();
	};
}

#endif
