#ifndef 3YP_UTIL_ASSETUTILS_HPP_
#define 3YP_UTIL_ASSETUTILS_HPP_

#include "render/MaterialLibrary.hpp"

namespace util {
	namespace AssetUtils {
		render::Material *getMaterial(int assetId, int materialId);
		void bindTexture(int assetId);
	}
}

#endif
