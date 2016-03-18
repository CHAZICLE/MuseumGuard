#ifndef __CONTROLSCHEME_H_INCLUDED__
#define __CONTROLSCHEME_H_INCLUDED__

namespace util {
	class DeltaTime;
}
class Entity;

namespace controls {
	class ControlScheme {
		protected:
			Entity *controlEntity;
		public:
			ControlScheme(Entity *controlEntity);
			~ControlScheme();
			virtual void tick(util::DeltaTime *deltaTime);
	};
};

#endif
