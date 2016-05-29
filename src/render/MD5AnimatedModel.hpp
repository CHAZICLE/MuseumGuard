#ifndef __MD5ANIMATEDMODEL_H_INCLUDED__
#define __MD5ANIMATEDMODEL_H_INCLUDED__

namespace render {
	class RenderManager;
	typedef struct MD5AnimatedModelJoint MD5AnimatedModelJoint;
	typedef struct MD5AnimatedModelBound MD5AnimatedModelBound;
}

#include "render/MD5Model.hpp"
#include "util/AssetManager.hpp"

namespace render {
	struct MD5AnimatedModelJoint {
		std::string name;
		int parent;
		int flags;
		int startIndex;
	};
	struct MD5AnimatedModelBound {
		float minX,minY,minZ,maxX,maxY,maxZ;
	};
	class MD5AnimatedModel : public util::Asset {
		public:
			MD5AnimatedModel(int assetId, std::istream &fp);
			~MD5AnimatedModel();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void render(render::RenderManager &manager, MD5Model &model, float time);
		private:
			int numFrames;
			int numJoints;
			int frameRate;
			int numAnimatedComponents;
			std::vector<MD5AnimatedModelJoint> hierarchy;
			std::vector<MD5AnimatedModelBound> bounds;
			Skeleton baseFrame;
			std::vector<Skeleton> frames;
			float *frameData;
	};
}

#endif
