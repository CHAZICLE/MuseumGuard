#ifndef __MD5MODEL_H_INCLUDED__
#define __MD5MODEL_H_INCLUDED__

namespace render{
	class MD5Model;
	typedef struct MD5Joint MD5Joint;
	typedef struct MD5Vertex MD5Vertex;
	typedef struct MD5Primitive MD5Primitive;
	typedef struct MD5Weight MD5Weight;
	typedef struct MD5Mesh MD5Mesh;
}

#include <string>
#include <fstream>
#include <list>
#include <glm/glm.hpp>
#include "util/AssetManager.hpp"

std::ostream &operator<<(std::ostream &ost, const render::MD5Model &model);
std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint);

namespace render {

	struct MD5Joint {
		std::string name;
		int parent;
		glm::vec3 pos;
		glm::vec3 ori;
	};

	struct MD5Vertex {
		int index;
		glm::vec2 tex;
		int startWeight;
		int countWeight;
	};

	struct MD5Primitive {
		int index;
		int vertex0;
		int vertex1;
		int vertex2;
	};

	struct MD5Weight {
		int index;
		int joint;
		float bias;
		glm::vec3 pos;
	};

	struct MD5Mesh {
		std::string shader_name;
		std::list<MD5Vertex> verts;
		std::list<MD5Primitive> tris;
		std::list<MD5Weight> weights;
	};

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
		private:
			std::list<MD5Joint> joints;
			std::list<MD5Mesh> meshes;
		friend std::ostream &::operator<<(std::ostream &ost, const render::MD5Model &model);
	};
}


#endif
