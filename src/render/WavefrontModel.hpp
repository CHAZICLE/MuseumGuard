#ifndef __WAVEFRONTMODEL_H_INCLUDED__
#define __WAVEFRONTMODEL_H_INCLUDED__

#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "util/AssetManager.hpp"
#include "util/gl.h"

namespace render {
	typedef struct {
		std::string object_name;
		bool usemtl;
		bool s;
		int lenf;
		int *f;
	} WavefrontObject;
	class WavefrontModel : public util::Asset {
		public:
			WavefrontModel(int assetId, std::istream &fp);
			virtual ~WavefrontModel();
			virtual void postload();
		private:
			std::list<WavefrontObject> objects;
	};
}

#endif
