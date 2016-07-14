#include "util/Boundaries.hpp"

#include "Player.hpp"


using namespace world;
using namespace entities;
using namespace util::Boundaries;

Player::Player() : super()
{
	this->doRender = false;
	this->bounds = new AABB(0.f, 0.f, 0.f, 0.4f, 0.4f, 0.4f);
}
Player::~Player()
{
	
}
