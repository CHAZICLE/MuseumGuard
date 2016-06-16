#ifndef __MD5MODEL_H_INCLUDED__
#define __MD5MODEL_H_INCLUDED__

#define ENABLE_DEBUG_RENDER_MD5JOINT

#include <vector>

namespace render{
	class MD5Model;
	typedef struct MD5Joint MD5Joint;
	typedef struct MD5Bone MD5Bone;
	typedef struct MD5Vertex MD5Vertex;
	typedef struct MD5Primitive MD5Primitive;
	typedef struct MD5Weight MD5Weight;
	typedef struct MD5Mesh MD5Mesh;
	typedef std::vector<MD5Bone> Skeleton;
}

#include <string>
#include <fstream>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "util/AssetManager.hpp"
#include "render/MaterialLibrary.hpp"
#ifdef ENABLE_DEBUG_RENDER_MD5JOINT
#include "render/RenderManager.hpp"
#endif

std::ostream &operator<<(std::ostream &ost, const render::MD5Model &model);
std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint);
std::ostream &operator<<(std::ostream &ost, const render::MD5Mesh &mesh);
std::ostream &operator<<(std::ostream &ost, const render::MD5Vertex &vert);
std::ostream &operator<<(std::ostream &ost, const render::MD5Weight &weight);

namespace render {

	struct MD5Joint {
		int index;
		std::string name;
		int parent;
	};

	struct MD5Bone {
		glm::vec3 pos;
		glm::quat ori;
	};

	struct MD5Vertex {
		int index;
		int startWeight;
		int countWeight;
	};

	struct MD5Weight {
		int index;
		int joint;
		float bias;
		glm::vec3 pos;
	};

	struct MD5Mesh {
		int mtlAssetId;
		int materialId;
		std::vector<MD5Vertex> verts;
		std::vector<GLfloat> textureUVs;
		std::vector<GLuint> indecies;
		std::vector<MD5Weight> weights;
		GLuint vertexBufferID,indexBufferID,vertexTextureBufferID;
		GLuint vertexColorBufferID;//debug
	};

	void calculateQuaternionW(glm::quat &q);

	//joints.append((name, parent, pos, ori))
	//meshes.append((shader_name, numverts, verts, numtris, tris, numweights, weights))

	//verts.append((vertIndex, tex, startWeight, countWeight))
	//tris.append((triIndex, vertIndex0, vertIndex1, vertIndex2))
	//weights.append((weightIndex, joint, bias, pos))


	class MD5Model : public util::Asset {
		public:
			MD5Model(int assetId, std::istream &fp);
			virtual ~MD5Model();
			void render();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
//#ifdef ENABLE_DEBUG_RENDER_MD5JOINT
			void renderSkeleton(render::RenderManager &manager, const Skeleton &skeleton);
			void renderWeights(render::RenderManager &manager, const Skeleton &skeleton);
//#endif
			void render(render::RenderManager &manager);
			void render(render::RenderManager &manager, const Skeleton &skeleton);
			void render(render::RenderManager &manager, const Skeleton &skeleton, MD5Mesh &mesh, const Material *material);
		//private:
			std::vector<MD5Joint> joints;
			std::vector<MD5Bone> bindPoseSkeleton;
			std::list<MD5Mesh> meshes;
			GLuint vertexArrayID;
		friend std::ostream &::operator<<(std::ostream &ost, const render::MD5Model &model);
	};
}


#endif
