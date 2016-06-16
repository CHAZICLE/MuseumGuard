#ifndef __SKELETALANIMATION_H_INCLUDED__
#define __SKELETALANIMATION_H_INCLUDED__

namespace render {
	class RenderManager;
	typedef struct SkeletalAnimationJoint SkeletalAnimationJoint;
	typedef struct SkeletalAnimationBound SkeletalAnimationBound;
}

#include "render/SkeletalModel.hpp"
#include "util/AssetManager.hpp"

namespace render {
	struct SkeletalAnimationJoint {
		std::string name;
		int parent;
		int flags;
		int startIndex;
	};
	struct SkeletalAnimationBound {
		float minX,minY,minZ,maxX,maxY,maxZ;
	};
	class SkeletalAnimation : public util::Asset {
		public:
			SkeletalAnimation(int assetId, std::istream &fp);
			~SkeletalAnimation();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void render(render::RenderManager &manager, SkeletalModel &model, float time);
		private:
			int numFrames;
			int numJoints;
			int frameRate;
			int numAnimatedComponents;
			std::vector<SkeletalAnimationJoint> hierarchy;
			std::vector<SkeletalAnimationBound> bounds;
			Skeleton baseFrame;
			std::vector<Skeleton> frames;
			float *frameData;
	};
}

#endif
