#include "util/Globals.hpp"
#include <istream>
#include "util/gl.h"

#include "DDSImage.hpp"

using namespace render;

DDSImage::DDSImage(int assetId, std::istream &fp) : util::Asset(assetId)
{
	this->imagePushed = false;
	this->header = new DDS_HEADER;
	fp.read((char *)this->header, sizeof(DDS_HEADER));
	this->imageDataSize = this->header->dwMipMapCount > 0 ? this->header->dwPitchOrLinearSize * 2 : this->header->dwPitchOrLinearSize;
	this->imageData = new char[this->imageDataSize];
	fp.read(this->imageData, this->imageDataSize*sizeof(char));
	unsigned int components  = 4;
	switch(this->header->ddspf.dwFourCC)
	{
	case DWFOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		components = 3;
		break;
	case DWFOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case DWFOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		util::Globals::fatalError("Unrecognised dds texture format for asset #"+std::to_string(assetId));
	}
}
DDSImage::~DDSImage()
{
	
}
void DDSImage::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".dds] " << (this->imagePushed ? "pushed to GPU" : "loaded and waiting") << " (line, " << this->header->dwWidth << ", " << this->header->dwHeight << ")";
}
void DDSImage::postload()
{
	glGenTextures(1, &this->textureID);
	this->bindTexture();

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	unsigned int width = this->header->dwWidth;
	unsigned int height = this->header->dwHeight;

	/* load the mipmaps */
	for (unsigned int level = 0; level < this->header->dwMipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 
		    0, size, this->imageData + offset);
		offset += size;
		width  /= 2;
		height /= 2;
	}
	delete [] this->imageData;
	this->imagePushed = true;
}
void DDSImage::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}
