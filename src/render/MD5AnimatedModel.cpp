#include "util/StreamUtils.hpp"

#include "MD5AnimatedModel.hpp"

using namespace render;
using namespace util::StreamUtils;

MD5AnimatedModel::MD5AnimatedModel(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));
	numFrames = readInt(fp);
	numJoints = readInt(fp);
	frameRate = readInt(fp);
	numAnimatedComponents = readInt(fp);
	//hierarchy
	for(int i=0;i<numJoints;i++)
	{
		MD5AnimatedModelJoint joint;
		joint.name = readString(fp);
		joint.parent = readInt(fp);
		joint.flags = readInt(fp);
		joint.startIndex = readInt(fp);
		this->hierarchy.push_back(joint);
	}
	// bounds
	for(int i=0;i<numFrames;i++)
	{
		MD5AnimatedModelBound bound;
		bound.minX = readFloat(fp);
		bound.minY = readFloat(fp);
		bound.minZ = readFloat(fp);
		bound.maxX = readFloat(fp);
		bound.maxY = readFloat(fp);
		bound.maxZ = readFloat(fp);
		this->bounds.push_back(bound);
	}
	// base frame
	for(int i=0;i<numJoints;i++)
	{
		MD5AnimatedModelFrame frame;
		frame.posX = readFloat(fp);
		frame.posY = readFloat(fp);
		frame.posZ = readFloat(fp);
		frame.oriX = readFloat(fp);
		frame.oriY = readFloat(fp);
		frame.oriZ = readFloat(fp);
		this->frames.push_back(frame);
	}
	// frames
	frameData = new float[numFrames*numAnimatedComponents];
	fp.read((char *)frameData, numFrames*numAnimatedComponents*sizeof(float));
	//[numFrames, numJoints, frameRate, numAnimatedComponents, hierarchy, bounds, baseframe, frames]
}
MD5AnimatedModel::~MD5AnimatedModel()
{
	
}
void MD5AnimatedModel::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".md5anim] " << this->numFrames << " frames (" << this->frameRate << " fps), " << this->numJoints << " joints, " << this->numAnimatedComponents << " animation components";
}
void MD5AnimatedModel::postload()
{
	
}
