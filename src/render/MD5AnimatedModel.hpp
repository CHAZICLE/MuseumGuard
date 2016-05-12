#ifndef __MD5ANIMATEDMODEL_H_INCLUDED__
#define __MD5ANIMATEDMODEL_H_INCLUDED__

namespace util {
	class Asset;
}
namespace render {
	typedef struct MD5AnimatedModelJoint MD5AnimatedModelJoint;
	typedef struct MD5AnimatedModelBound MD5AnimatedModelBound;
	typedef struct MD5AnimatedModelFrame MD5AnimatedModelFrame;
}

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
	struct MD5AnimatedModelFrame {
		float posX,posY,posZ,oriX,oriY,oriZ;
	};
	class MD5AnimatedModel : public util::Asset {
		public:
			MD5AnimatedModel(int assetId, std::istream &fp);
			~MD5AnimatedModel();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
		private:
			int numFrames;
			int numJoints;
			int frameRate;
			int numAnimatedComponents;
			std::vector<MD5AnimatedModelJoint> hierarchy;
			std::vector<MD5AnimatedModelBound> bounds;
			std::vector<MD5AnimatedModelFrame> frames;
			float *frameData;
	};
}

#endif
