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

	struct MaterialAsset {
		int assetId;
		int materialId;
	};

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
		int map_Ka;
		int map_Kd;
		int map_Ks;
		int map_Ns;
		int map_d;
		int disp;
		int decal;
		int bump;
	};

	class MaterialLibrary : public util::Asset {
		public:
			MaterialLibrary(int assetId, std::istream &fp);
			~MaterialLibrary();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void printMaterial(std::ostream &ost);
			Material *getMaterial(int materialId);
		private:
			std::vector<Material> materials;
	};
}

#endif
