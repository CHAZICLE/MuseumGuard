#ifndef __STATICMODEL_H_INCLUDED__
#define __STATICMODEL_H_INCLUDED__

namespace render {
	struct FaceKey;
	struct FaceKeyHasher;
	typedef struct StaticModelObject StaticModelObject;
	class RenderManager;
	class StaticModel;
}

#include <vector>
#include <glm/glm.hpp>
#include "util/AssetManager.hpp"
#include "util/gl.h"
#include "world/collisions/StaticMesh.hpp"

std::ostream &operator<<(std::ostream &ost, const render::StaticModel &model);
std::ostream &operator<<(std::ostream &ost, const render::StaticModelObject &o);

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
	struct StaticModelObject {
		std::string name;
		int mtlAsset;
		int materialId;
		bool s;
		int numPrimitives;//triangles/faces
		//int numVerticies = numPrimitives*3
		GLuint *indecies;//length = numVerticies
		GLuint indexBufferID;
	};
	class StaticModel : public util::Asset {
		friend class world::collisions::StaticMesh;
		friend std::ostream &::operator<<(std::ostream &ost, const render::StaticModel &model);
		public:
			StaticModel(int assetId, std::istream &fp);
			virtual ~StaticModel();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			util::Boundaries::AABB &getBounds();
			void render(render::RenderManager &rManager, int shader);
		private:
			util::Boundaries::AABB *bounds;
			int dataBufferStride;
			int dataBufferNormalsOffset;
			int dataBufferColorsOffset;
			int lenVertexPositions;
			std::vector<GLfloat> dataBuffer;
			std::list<StaticModelObject *> objects;
			GLuint vertexArrayID,vertexDataBufferID,tempColorBuffer;

			int temp_totalVertexCount;
	};
}


#endif
