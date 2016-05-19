#ifndef __DDSIMAGE_H_INCLUDED__
#define __DDSIMAGE_H_INCLUDED__

#define DWFOURCC_DXT1 0x31545844
#define DWFOURCC_DXT3 0x33545844
#define DWFOURCC_DXT5 0x35545844

namespace render {
	class DDSImage;
	typedef struct DDS_HEADER DDS_HEADER;
	typedef struct DDS_PIXELFORMAT DDS_PIXELFORMAT;
	typedef unsigned int DWORD;
}

#include "util/gl.h"
#include "util/AssetManager.hpp"

namespace render {
	struct DDS_PIXELFORMAT {
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwFourCC;
		DWORD dwRGBBitCount;
		DWORD dwRBitMask;
		DWORD dwGBitMask;
		DWORD dwBBitMask;
		DWORD dwABitMask;
	};
	struct DDS_HEADER {
		DWORD           dwSize;
		DWORD           dwFlags;
		DWORD           dwHeight;
		DWORD           dwWidth;
		DWORD           dwPitchOrLinearSize;
		DWORD           dwDepth;
		DWORD           dwMipMapCount;
		DWORD           dwReserved1[11];
		DDS_PIXELFORMAT ddspf;
		DWORD           dwCaps;
		DWORD           dwCaps2;
		DWORD           dwCaps3;
		DWORD           dwCaps4;
		DWORD           dwReserved2;
	};
	class DDSImage : public util::Asset {
		public:
			DDSImage(int assetId, std::istream &fp);
			virtual ~DDSImage();
			virtual void write(std::ostream &ost) const;
			virtual void postload();
			void bindTexture();
		private:
			bool imagePushed;
			DDS_HEADER *header;
			int imageDataSize;
			char *imageData;
			GLuint textureID;
			GLint format;
	};
}

#endif
