#ifndef __MATERIALLIBRARY_H_INCLUDED__
#define __MATERIALLIBRARY_H_INCLUDED__

namespace render {
	class MaterialLibrary;
	typedef struct Material Material;
}

#include "render/MaterialData.h"
#include "util/AssetManager.hpp"
#include <vector>
#include <ostream>

std::ostream &operator<<(std::ostream &ost, const render::Material &m);

namespace render {

	struct Material {
		std::string name;
		int flags;

		float Ka[3];
		float Kd[3];
		float Ks[3];
		float Tf[3];
		float d;
		float Ns;
		float Ni;
		int illum;
		int sharpness;
		std::string map_Ka;
		std::string map_Kd;
		std::string map_Ks;
		std::string map_Ns;
		std::string map_d;
		std::string disp;
		std::string decal;
		std::string bump;
	};

	class MaterialLibrary : public util::Asset {
		public:
			MaterialLibrary(int assetId, std::istream &fp);
			~MaterialLibrary();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void printMaterial(std::ostream &ost);
		private:
			std::vector<Material> materials;
	};
}

#endif
