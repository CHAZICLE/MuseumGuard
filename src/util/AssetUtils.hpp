#ifndef __ASSETUTILS_H_INCLUDED__
#define __ASSETUTILS_H_INCLUDED__

#include "render/MaterialLibrary.hpp"

namespace util {
	namespace AssetUtils {
		render::Material *getMaterial(int assetId, int materialId);
		void bindTexture(int assetId);
	}
}

#endif
