#include "util/StreamUtils.hpp"

//d
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "render/BasicShapes.hpp"
#include "util/gl.h"
#include "render/shaders/ShaderUtils.hpp"

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
		MD5Bone bone;
		bone.pos.x = readFloat(fp);
		bone.pos.y = readFloat(fp);
		bone.pos.z = readFloat(fp);
		bone.ori.x = readFloat(fp);
		bone.ori.y = readFloat(fp);
		bone.ori.z = readFloat(fp);
		calculateQuaternionW(bone.ori);
		baseFrame.push_back(bone);
	}
	// frames
	frameData = new float[numFrames*numAnimatedComponents];
	fp.read((char *)frameData, numFrames*numAnimatedComponents*sizeof(float));
	// frames>collate
	int j = 0;
	for(int f=0;f<numFrames;f++)
	{
		Skeleton skeleton;
		for(int i=0;i<numJoints;i++)
		{
			MD5AnimatedModelJoint &joint = this->hierarchy[i];
			MD5Bone bone = this->baseFrame[i];
			j = 0;
			float *currentFrameData = &frameData[f*numAnimatedComponents];
			if(joint.flags& 1) bone.pos.x = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags& 2) bone.pos.y = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags& 4) bone.pos.z = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags& 8) bone.ori.x = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags&16) bone.ori.y = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags&32) bone.ori.z = currentFrameData[joint.startIndex + (j++)];
			if(joint.flags&(8|16|32))
				calculateQuaternionW(bone.ori);
			if(joint.parent>=0)
			{
				MD5Bone &parentBone = skeleton[joint.parent];
				bone.pos = parentBone.pos + parentBone.ori*bone.pos;
				bone.ori = parentBone.ori*bone.ori;
				bone.ori = glm::normalize(bone.ori);
			}
			skeleton.push_back(bone);
		}
		this->frames.push_back(skeleton);
	}
	delete [] frameData;
	frameData = 0;
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
#include "render/BasicShapes.hpp"
void MD5AnimatedModel::render(render::RenderManager &manager, MD5Model &model, float time)
{
	// scale up
	glm::mat4 tempV = manager.V;
	manager.V = glm::scale(manager.V, glm::vec3(10.f, 10.f, 10.f));
	manager.markVDirty();
	// time scale up
	float totalAnimationTime = frameRate*time;
	int frame = (int)std::fmod(totalAnimationTime,numFrames);
	Skeleton &skeleton = this->frames[frame];
	model.render(manager, skeleton);
	model.renderSkeleton(manager, skeleton);
	model.renderWeights(manager, skeleton);


	manager.disableCullFace();
	MD5AnimatedModelBound &aabb = this->bounds[frame];
	manager.M = glm::mat4(1.0f);
	manager.M = glm::translate(manager.M, glm::vec3(aabb.minX, aabb.minY, aabb.minZ));
	manager.M = glm::scale(manager.M, glm::vec3(aabb.maxX-aabb.minX, aabb.maxY-aabb.minY, aabb.maxZ-aabb.minZ));
	manager.markMDirty();
	manager.setMVPMatrix(shaders::program_solidcolor_MVP);
	BasicShapes::renderUnitCube(shaders::program_solidcolor_vertexPosition);
	manager.enableCullFace();

	manager.V = tempV;
	manager.markVDirty();
}
