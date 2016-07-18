#include "util/Globals.hpp"
#include <istream>
#include <cstring>
#include "util/gl.h"
#include "util/StreamUtils.hpp"
#include <SOIL/SOIL.h>

#include "DDSImage.hpp"

using namespace render;
using namespace util::StreamUtils;

DDSImage::DDSImage(int assetId, std::istream &fp) : util::Asset(assetId)
{
	this->setName(readString(fp));
	
	// Read DDS from asset stream
	this->imageDataSize = readInt(fp);
	this->imageData = new unsigned char[this->imageDataSize];
	fp.read((char *)this->imageData, this->imageDataSize*sizeof(char));
}
DDSImage::~DDSImage()
{
	
}
void DDSImage::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".dds] " << (this->imagePushed ? "pushed to GPU" : "loaded and waiting");
}
void DDSImage::postload()
{
	this->textureID = SOIL_load_OGL_texture_from_memory
		(
			this->imageData,
			this->imageDataSize,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
		);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLfloat largest_supported_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
}
void DDSImage::bindTexture()
{
	//std::cout << getName() << " bind" << std::endl;
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}
