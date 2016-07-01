#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <glm/vec3.hpp>
#include <list>
#include "input/ControlScheme.hpp"
#include "input/Controls.hpp"
#include "render/StaticModel.hpp"

namespace world {
	class World;
	namespace entities {
		class Enemy;
		class Player;
		class SecurityCamera;
		class Turret;
	}
}
namespace render {
	class RenderManager;
}
namespace util {
	class DeltaTime;
}
namespace controls {
	class ControlScheme;
}

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
		glm::vec3 viewDirection, viewUp;
	public:
		World();
		~World();
		void add(Entity *ent, glm::vec3 location);
		void tick(util::DeltaTime &deltaTime, bool surface);
		void render(render::RenderManager &manager, bool isSurface);
		void onDebugControl(Control control, int action);
		int remainingTurrets;
		int remainingCameras;
	};
}

#endif
