#ifndef __MATERIALLIBRARY_H_INCLUDED__
#define __MATERIALLIBRARY_H_INCLUDED__

#define MASK_Ka 1
#define MASK_Kd 2
#define MASK_Ks 4
#define MASK_Tf 8
#define MASK_d 16
#define MASK_Ns 32
#define MASK_Ni 64
#define MASK_illum 128
#define MASK_sharpness 256
#define MASK_map_Ka 512
#define MASK_map_Kd 1024
#define MASK_map_Ks 2048
#define MASK_map_Ns 4096
#define MASK_map_d 8192
#define MASK_disp 16384
#define MASK_decal 32768
#define MASK_bump 65536

namespace render {
	class MaterialLibrary;
	typedef struct Material Material;
}


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
