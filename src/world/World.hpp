#ifndef 3YP_WORLD_WORLD_HPP_
#define 3YP_WORLD_WORLD_HPP_

#include <list>
#include "render/StaticModel.hpp"

#include "world/entities/Enemy.hpp"
#include "world/entities/Player.hpp"
#include "world/entities/Turret.hpp"
#include "world/entities/SecurityCamera.hpp"

#include "render/RenderManager.hpp"

#include "util/DeltaTime.hpp"

#include "input/Controls.hpp"
#include "input/ControlScheme.hpp"

#include "ai/path/NavigationGraph.hpp"

#define SELECTOR_OFF 0
#define SELECTOR_SECURITY_CAMERA 1
#define SELECTOR_TURRET 2
#define SELECTOR_REMOVE 3

namespace world {
	class World {
	private:
		// World models
		render::StaticModel *world_interactive_model;
		render::StaticModel *world_aesthetic_model;
		render::StaticModel *world_skybox;
		collisions::StaticMesh *world_interactive_collision;
		
		// Selector
		int selector;
		bool enableSelector;
		world::entities::SecurityCamera *selectorCamera;
		world::entities::Turret *selectorTurret;
		world::Entity *selectorRemove;
		
		std::list<world::Entity *> entities;
		std::list<world::Entity *> spawningEntities;
		
		world::entities::Player *player;
		world::entities::Enemy *enemy;
		
		controls::ControlScheme *controlScheme;
		
		double vertAngle, horizAngle, lastX, lastY;
		float selectorYaw;
		glm::vec3 viewDirection, viewUp;
	public:
		ai::path::NavigationGraph *world_navigation_graph;
		World();
		~World();
		void add(Entity *ent, glm::vec3 location);
		void tick(util::DeltaTime &deltaTime, bool surface);
		void render(render::RenderManager &manager, bool isSurface);
		void onDebugControl(Control control, int action);
		void onRayHit(glm::vec3 rayOrigin, float distance, glm::vec3 normal);
		int remainingTurrets;
		int remainingCameras;
	};
}

#endif
