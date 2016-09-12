#include "util/Boundaries.hpp"
#include "world/World.hpp"

#include "Player.hpp"


using namespace world;
using namespace entities;
using namespace util::Boundaries;

Player::Player() : super()
{
	this->doRender = false;
	this->bounds = new AABB(0.f, 0.f, 0.f, 0.4f, 0.4f, 0.4f);
	this->setBulletHealth(1.f, 2);
}
Player::~Player()
{
	
}
void Player::die(double time, glm::vec3 direction, int type)
{
	this->getWorld().gameOver(GAME_OVER_PLAYER_DESTROYED);
}
