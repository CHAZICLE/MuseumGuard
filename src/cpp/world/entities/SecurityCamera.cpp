#include <cmath>
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "util/gl.h"

#include "render/SkeletalAnimation.hpp"
#include "render/BasicShapes.hpp"
#include "render/DDSImage.hpp"

#include "world/entities/Enemy.hpp"

#include "res/md5/security_camera.md5mesh.h"

#include "SecurityCamera.hpp"

using namespace world;
using namespace entities;
using namespace render;
using namespace ai;

SecurityCamera::SecurityCamera() : super()
{
	this->model = (SkeletalModel *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5MESH);
	this->perception = new PerceptionManager(this, {&typeid(Enemy)}, this->model->bindPoseSkeleton[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_CAMERASTALK].pos);
	this->perception->setYawBounds(-M_PI/2, M_PI/2);
	this->initAnimation = (SkeletalAnimation *)util::AssetManager::getAssetManager()->getAsset(ASSET_SECURITY_CAMERA_MD5ANIM);
	this->warningImage = (render::DDSImage *)util::AssetManager::getAssetManager()->getAsset(ASSET_ATTENTION_PNG);
	this->animationDuration = this->initAnimation->getAnimationDuration();
	this->animationCurrent = 0;
	this->initAnimating = true;
	this->selector = false;
	this->bounds = &this->initAnimation->getFrameBounds(-1);

//	// Setup billboard
//	glGenVertexArrays(1, &this->billboardVertexArrayId);
//	glBindVertexArray(this->billboardVertexArrayId);
//	glGenBuffers(1, &this->billboardVertexPositionBufferID);
//	glBindBuffer(GL_ARRAY_BUFFER, this->billboardVertexPositionBufferID);
//	static const GLfloat g_vertex_buffer_data[] = {
//		1.f, 1.f,
//		-1.f, 1.f,
//		-1.f, -1.f,
//		1.f, -1.f,
//	};
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}
SecurityCamera::~SecurityCamera()
{
	
}

#define TRACKING_SPEED 10

void SecurityCamera::tick(util::DeltaTime &deltaTime)
{
	if(this->selector)
		return;
	if(this->initAnimating)
	{
		if(deltaTime.getTime()-this->getSpawnTime()<=this->animationDuration)
		{
			this->animationCurrent = deltaTime.getTime()-this->getSpawnTime();
			this->bounds = &this->initAnimation->getFrameBounds(this->initAnimation->getFrame(this->animationCurrent));
		}
		else
		{
			this->initAnimating = false;
			this->bounds = &this->initAnimation->getFrameBounds(-1);
		}
	}
	else
	{
		this->perception->tick(deltaTime);
		if(this->perception->getTargetEntity()!=0)
		{
			this->warningStop = deltaTime.getTime()+3;
			this->warning = true;
		}
		else
		{
			this->warning = deltaTime.getTime()<this->warningStop;
		}
	}
}
void SecurityCamera::render(RenderManager &rManager)
{
	//this->setOrientation(this->getOrientation()*glm::quat(glm::vec3(0, 0, glm::radians((float)(std::rand()%100)/10))));
	
	
	rManager.pushMatrixM();
	rManager.M = glm::translate(rManager.M, this->getPosition())*glm::toMat4(this->getOrientation());
	rManager.markMDirty();
	rManager.useShader(SHADER_fuzzyModel);
	
	/*Skeleton skel = this->model->bindPoseSkeleton;
	this->model->render(rManager, skel);*/
	//std::cout << "R time=" << this->animTime << std::endl;
	//this->initAnimation->render(rManager, *this->model, this->animTime);
	//drone->renderSkeleton(rManager, skel);
	//drone->renderWeights(rManager, skel);
	if(this->selector)
	{
		this->model->render(rManager, this->initAnimation->getSkeleton(0));
	}
	else if(this->initAnimating)
	{
		this->model->render(rManager, this->initAnimation->getSkeleton(this->animationCurrent));
		//this->initAnimation->renderBounds(rManager, this->animationCurrent);
	}
	else
	{
		Skeleton skel = this->model->bindPoseSkeleton;
		skel[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_CAMERASTALK].ori = this->perception->getOrientation();
		this->model->render(rManager, skel);
		//this->initAnimation->renderBounds(rManager, this->animationCurrent);
		
		
		// Render billboard icon
		if(this->warning)
		{
			rManager.disableDepth();
			rManager.enableTransparency();
			
			render::shaders::ShaderProgram *prog = rManager.useShader(SHADER_billboard);
			glm::vec3 billpoc = this->getPosition()+this->getOrientation()*(skel[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_MOTIONSENSOR].pos+skel[ASSET_SECURITY_CAMERA_MD5MESH_JOINT_MOTIONSENSOR].ori*glm::vec3(0.f, 0.04f, 0.0f));
			glUniform3fv(prog->getShaderLocation(true, SHADER_billboard_billboard_center), 1, &billpoc[0]);

			glActiveTexture(GL_TEXTURE0);
			this->warningImage->bindTexture();
			glUniform1i(prog->getShaderLocation(true, SHADERVAR_material_map_Kd), 0);

			float sizeMM = 10.f;
			glm::vec2 scaler = glm::vec2(sizeMM/rManager.getWidthMM(), sizeMM/rManager.getHeightMM());
			glUniform2fv(prog->getShaderLocation(true, SHADER_billboard_screen_scaler), 1, &scaler[0]);

			rManager.disableCullFace();

			render::BasicShapes::renderUnitSquare(prog->getShaderLocation(false, SHADERVAR_vertex_position));
			rManager.enableCullFace();

			//glBindVertexArray(this->billboardVertexArrayId);
			//glBindBuffer(GL_ARRAY_BUFFER, this->billboardVertexPositionBufferID);
			//prog->setVertexAttributePointer(SHADERVAR_vertex_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
			//glDrawArrays(GL_QUADS, 0, 4);

			rManager.disableTransparency();
			rManager.enableDepth();
		}
	}
	//this->getBounds()->render(rManager, glm::vec4(1.f, 1.f, 0.f, 1.f), false);
	rManager.popMatrixM();

}

//controllable by player
//	update quat
//controllable by scripts
//	lookAt(vector)
//	keepLookingAt(Entity*)
