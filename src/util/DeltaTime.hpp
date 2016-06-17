#ifndef __DELTATIME_H_INCLUDED__
#define __DELTATIME_H_INCLUDED__

// c is u/s
// u/t = u/s * s/t
#define DELTATIME_ARITHMETIC_PROGRESSION(dt, c) ((dt)->getTimeDelta()*(c))
#define DELTATIME_GEOMETRIC_PROGRESSION(dt, c) ((dt)->getTimeDelta()*(c))

namespace util {
	class DeltaTime {
		double targetFramerate,currentTime,deltaTime,framerate,offsetTime;
		bool vsync;
		public:
			DeltaTime(bool vsync, double targetFramerate);
			~DeltaTime();
			void postTime(double time);
			void setOffsetTime(double offsetTime);
			double getTime();
			double getTimeDelta();
			double getFramerate();
			double getTargetFramerate();
			bool isVsync();
	};
}

#endif
