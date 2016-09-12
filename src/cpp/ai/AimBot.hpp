#ifndef FYP_AI_AIMBOT_HPP_
#define FYP_AI_AIMBOT_HPP_

#include <list>
#include "ai/PerceptionManager.hpp"
#include "world/Entity.hpp"
#include "util/DeltaTime.hpp"
#include "render/RenderManager.hpp"

namespace ai {
	struct Bullet {
		double startTime;
		glm::vec3 start;
		glm::vec3 end;
	};
	class AimBot {
		private:
			ai::PerceptionManager *perception;
			std::list<Bullet *> bullets;
			double
				bulletSpread,//radius spread
				bulletLifespan,//seconds bullets last
				bulletFireDelay,//seconds between bullets
				lastBulletFiredTime,//seconds since last bullets were fired
				perceptionFireDelay,//second between perceiving and firing
					tmp;
		public:
			AimBot(ai::PerceptionManager *perception);
			~AimBot();
			void tick(util::DeltaTime &deltaTime);
			void render(render::RenderManager &rManager);
	};
}

#endif
