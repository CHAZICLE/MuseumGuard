#ifndef __WAVEFRONTMODEL_H_INCLUDED__
#define __WAVEFRONTMODEL_H_INCLUDED__

namespace render {
	class RenderManager;
}

#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "util/AssetManager.hpp"
#include "util/gl.h"

namespace render {
	typedef struct {
		std::string name;
		std::string usemtl;
		bool s;
		int numPrimitives;//triangles/faces
		//int numVerticies = numPrimitives*3
		int *indecies;//length = numVerticies

		GLuint indexBufferID;
	} WavefrontObject;
	class WavefrontModel : public util::Asset {
		public:
			WavefrontModel(int assetId, std::istream &fp);
			virtual ~WavefrontModel();
			virtual void postload();
			void render(render::RenderManager *rManager);
		private:
			int dataBufferStride;
			std::vector<float> dataBuffer;
			std::list<WavefrontObject *> objects;
			GLuint vertexArrayID,vertexDataBufferID;
	};
}

#endif
