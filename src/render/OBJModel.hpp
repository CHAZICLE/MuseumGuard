#ifndef __OBJMODEL_H_INCLUDED__
#define __OBJMODEL_H_INCLUDED__

namespace render {
	struct FaceKey;
	struct FaceKeyHasher;
	typedef struct OBJObject OBJObject;
	class RenderManager;
	class OBJModel;
}

#include <vector>
#include <glm/glm.hpp>
#include "util/AssetManager.hpp"
#include "util/gl.h"

std::ostream &operator<<(std::ostream &ost, const render::OBJModel &model);
std::ostream &operator<<(std::ostream &ost, const render::OBJObject &o);

namespace render {
	struct FaceKey {
		int vertexPositionIndex;
		int vertexTextureIndex;
		int vertexNormalIndex;
		bool operator <(struct FaceKey &A) const {
			return this->vertexPositionIndex<A.vertexPositionIndex && this->vertexTextureIndex<A.vertexTextureIndex && this->vertexNormalIndex<A.vertexNormalIndex;
		}
		bool operator ==(const struct FaceKey &A) const {
			return this->vertexPositionIndex==A.vertexPositionIndex && this->vertexTextureIndex==A.vertexTextureIndex && this->vertexNormalIndex==A.vertexNormalIndex;
		}
	};
	struct FaceKeyHasher {
		std::size_t operator()(const struct render::FaceKey& k) const
		{
			using std::size_t;
			return ((std::hash<int>()(k.vertexPositionIndex)^ (std::hash<int>()(k.vertexTextureIndex) << 1)) >> 1) ^ (std::hash<int>()(k.vertexNormalIndex) << 1);
		}
	};
	struct OBJObject {
		std::string name;
		std::string usemtl;
		bool s;
		int numPrimitives;//triangles/faces
		//int numVerticies = numPrimitives*3
		GLuint *indecies;//length = numVerticies
		GLuint indexBufferID;
	};
	class OBJModel : public util::Asset {
		friend std::ostream &::operator<<(std::ostream &ost, const render::OBJModel &model);
		public:
			OBJModel(int assetId, std::istream &fp);
			virtual ~OBJModel();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void render(render::RenderManager *rManager, GLuint shaderVertexPositionID);
		private:
			int dataBufferStride;
			int dataBufferTexturesOffset;
			int dataBufferNormalsOffset;
			std::vector<GLfloat> dataBuffer;
			std::list<OBJObject *> objects;
			GLuint vertexArrayID,vertexDataBufferID;
	};
}


#endif
