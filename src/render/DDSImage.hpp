#ifndef FYP_RENDER_DDSIMAGE_HPP_
#define FYP_RENDER_DDSIMAGE_HPP_

#define DWFOURCC_DXT1 0x31545844
#define DWFOURCC_DXT3 0x33545844
#define DWFOURCC_DXT5 0x35545844

namespace render {
	class DDSImage;
}

#include "util/gl.h"
#include "util/AssetManager.hpp"

namespace render {
	class DDSImage : public util::Asset {
		public:
			DDSImage(int assetId, std::istream &fp);
			virtual ~DDSImage();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void bindTexture();
		private:
			bool imagePushed;
			int imageDataSize;
			unsigned char *imageData;
			GLuint textureID;
			GLint format;
	};
}

#endif
