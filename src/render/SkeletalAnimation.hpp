#ifndef 3YP_RENDER_SKELETALANIMATION_HPP_
#define 3YP_RENDER_SKELETALANIMATION_HPP_

namespace render {
	class RenderManager;
	typedef struct SkeletalAnimationJoint SkeletalAnimationJoint;
	typedef struct SkeletalAnimationBound SkeletalAnimationBound;
}

#include "render/SkeletalModel.hpp"
#include "util/AssetManager.hpp"
#include "util/Boundaries.hpp"

namespace render {
	struct SkeletalAnimationJoint {
		std::string name;
		int parent;
		int flags;
		int startIndex;
	};
	class SkeletalAnimation : public util::Asset {
		public:
			SkeletalAnimation(int assetId, std::istream &fp);
			~SkeletalAnimation();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void renderBounds(RenderManager &rManager, double time);
			double getAnimationDuration();
			int getFrame(double time);
			Skeleton getFrameSkeleton(int frame);
			util::Boundaries::AABB &getFrameBounds(int frame);
			Skeleton getInterpolatedSkeleton(int firstFrame);
			Skeleton getSkeleton(double time);
		private:
			int numFrames;
			int numJoints;
			int frameRate;
			int numAnimatedComponents;
			std::vector<SkeletalAnimationJoint> hierarchy;
			std::vector<util::Boundaries::AABB *> bounds;
			Skeleton baseFrame;
			std::vector<Skeleton> frames;
			float *frameData;
	};
}

#endif
