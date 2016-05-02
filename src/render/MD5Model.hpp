#ifndef __MD5MODEL_H_INCLUDED__
#define __MD5MODEL_H_INCLUDED__

namespace render{
	class MD5Model;
}

#include <string>
#include <fstream>
#include <list>
#include <glm/glm.hpp>

namespace render {

	typedef struct {
		std::string name;
		int parent;
		glm::vec3 pos;
		glm::vec3 ori;
	} MD5Joint;

	typedef struct {
		int index;
		glm::vec2 tex;
		int startWeight;
		int countWeight;
	} MD5Vertex;

	typedef struct {
		int index;
		int vertex0;
		int vertex1;
		int vertex2;
	} MD5Tri;

	typedef struct {
		int index;
		int joint;
		float bias;
		glm::vec3 pos;
	} MD5Weight;

	typedef struct {
		std::string shader_name;
		std::list<MD5Vertex> verts;
		std::list<MD5Tri> tris;
		std::list<MD5Weight> weights;
	} MD5Mesh;

	//joints.append((name, parent, pos, ori))
	//meshes.append((shader_name, numverts, verts, numtris, tris, numweights, weights))

	//verts.append((vertIndex, tex, startWeight, countWeight))
	//tris.append((triIndex, vertIndex0, vertIndex1, vertIndex2))
	//weights.append((weightIndex, joint, bias, pos))


	class MD5Model {
		public:
			MD5Model(std::istream &fp);
			~MD5Model();
			void render();
		private:
			std::list<MD5Joint> joints;
	};
}

std::ostream &operator<<(std::ostream &ost, const render::MD5Joint &joint);

#endif
