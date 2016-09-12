#include <cmath>
#include "world/World.hpp"
#include "util/Boundaries.hpp"
#include "render/BasicShapes.hpp"

#include "AimBot.hpp"

using namespace ai;
using namespace world;
using namespace util::Boundaries;

AimBot::AimBot(ai::PerceptionManager *perception)
{
	this->perception = perception;
	this->bulletSpread = glm::radians(2.f);
	this->bulletLifespan = 0.02f;
	this->bulletFireDelay = 0.43f;
	this->lastBulletFiredTime = -1.f;
	this->perceptionFireDelay = 1.f;

	//this->bulletLifespan = 0.001f;
	//this->bulletFireDelay = 0.001f;
	//this->bulletLifespan = this->bulletFireDelay;
}
AimBot::~AimBot()
{
	
}
void AimBot::tick(util::DeltaTime &deltaTime)
{
	if(this->perception->getPerceivedEntity()!=0 && (this->lastBulletFiredTime<0 || deltaTime.getTime()>this->lastBulletFiredTime+this->bulletFireDelay))
	{
		Raycast ray;
		ray.origin = this->perception->getEyePosition();
		ray.direction = (
					this->perception->getOrientation()
					*glm::angleAxis(static_cast<float>(std::fmod(std::rand(),this->bulletSpread*2)-this->bulletSpread), glm::vec3(1,0,0))
					*glm::angleAxis(static_cast<float>(std::fmod(std::rand(), this->bulletSpread*2)-this->bulletSpread), glm::vec3(0,0,1))
				)*glm::vec3(0,1,0);
		ray.maxDistance = 0;
		Entity *e = this->perception->getOriginEntity();
		RaycastResult result = this->perception->getOriginEntity()->getWorld().rayCast(ray, &e);
		Bullet *bullet = new Bullet;
		this->lastBulletFiredTime = bullet->startTime = deltaTime.getTime();
		bullet->start = ray.origin;
		bullet->end = ray.origin+ray.direction*(result.hit ? result.distance : 1000.f);
		this->bullets.push_back(bullet);
	}
	for(std::list<Bullet *>::iterator it = this->bullets.begin();it!=this->bullets.end();it++)
	{
		Bullet *b = *it;
		if(deltaTime.getTime()-b->startTime>this->bulletLifespan)
		{
			it = this->bullets.erase(it);
			delete b;
		}
	}
}
void AimBot::render(render::RenderManager &rManager)
{
	rManager.M = glm::mat4(1.0f);
	rManager.markMDirty();
	render::shaders::ShaderProgram *shader = rManager.useShader(SHADER_solidColor);
	glUniform4f(shader->getShaderLocation(true, SHADER_solidColor_solidColor), 0.8f, 0.8f, 0.6f, 1.f);
	for(auto *b : this->bullets)
	{
		render::BasicShapes::drawLine(b->start, b->end, rManager.getVertexPosition());
	}
}
