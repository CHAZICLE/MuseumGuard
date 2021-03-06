#include "util/DeltaTime.hpp"
#include "util/gl.h"
#include <glm/gtc/quaternion.hpp>
#include "world/Entity.hpp"
#include <iostream>
#include "util/AssetManager.hpp"
#include "util/Boundaries.hpp"

#include "PlayerGameControls.hpp"

using namespace world;
using namespace controls;
using namespace collisions;
using namespace util::Boundaries;

Sphere *playerBounds;

PlayerGameControls::PlayerGameControls(Entity *controlEntity, StaticMesh *world) : ControlScheme(controlEntity)
{
	this->lastCursorX = -1000;
	this->lastCursorY = -1000;
	this->cursorX = 0;
	this->cursorY = 0;
	this->cursorDeltaX = 0;
	this->cursorDeltaY = 0;
	this->lastCursor = false;
	this->world = world;
	//playerBounds = new AABB(0,0,0, 0.2f,0.2f,0.2f);
	playerBounds = new Sphere;
	playerBounds->radius = 0.2f;
}
PlayerGameControls::~PlayerGameControls()
{
	
}
void PlayerGameControls::tick2(render::RenderManager *rManager, util::DeltaTime &deltaTime)
{
	float r;
	glm::vec3 movement = glm::vec3(0,0,0);
	glm::quat orientation = this->controlEntity->getOrientation();

	// Mouse controls
	GLFWwindow *window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &this->cursorX, &this->cursorY);
	if(!this->lastCursor)
	{
		this->lastCursor = true;
	}
	else
	{
		r = deltaTime.getTimeDelta()*2.f;
		this->cursorDeltaX = (this->cursorX-this->lastCursorX)*r;
		this->cursorDeltaY = (this->cursorY-this->lastCursorY)*r;
		if(this->cursorDeltaX!=0 || this->cursorDeltaY!=0)
		{
			glm::quat q = glm::quat(glm::vec3(0, 0, glm::radians(-this->cursorDeltaX)));
			glm::quat preq = glm::quat(glm::vec3(glm::radians(-this->cursorDeltaY), 0, 0));
			orientation = q*orientation*preq;
		}
	}
	this->lastCursorX = this->cursorX;
	this->lastCursorY = this->cursorY;

	// Orientation changes
	r = 2*deltaTime.getTimeDelta();

	// Relative Roll
	if(glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
		orientation = glm::angleAxis(r, glm::vec3( 0, 0,-1))*orientation;
	if(glfwGetKey(window, GLFW_KEY_3)==GLFW_PRESS)
		orientation = glm::angleAxis(r, glm::vec3( 0, 0, 1))*orientation;

	// Relative Pitch
	if(glfwGetKey(window, GLFW_KEY_R)==GLFW_PRESS)
		orientation = glm::angleAxis(r, glm::vec3(-1, 0, 0))*orientation;
	if(glfwGetKey(window, GLFW_KEY_V)==GLFW_PRESS)
		orientation = glm::angleAxis(r, glm::vec3( 1, 0, 0))*orientation;

	// Absolute Yaw
	if(glfwGetKey(window, GLFW_KEY_Z)==GLFW_PRESS)
		orientation *= glm::angleAxis(r, glm::vec3( 0, 1, 0));
	if(glfwGetKey(window, GLFW_KEY_C)==GLFW_PRESS)
		orientation *= glm::angleAxis(r, glm::vec3( 0,-1, 0));

	// Movement Controls
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
		r = 4.f*deltaTime.getTimeDelta();
	else
		r = 1.f*deltaTime.getTimeDelta();

	// Forward/Backward
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
		movement += orientation*glm::vec3( 0,  r, 0);
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
		movement += orientation*glm::vec3( 0, -r, 0);

	// Left/Right
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
		movement += orientation*glm::vec3(-r, 0, 0);
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
		movement += orientation*glm::vec3( r, 0, 0);

	// Down/Up
	if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS)
		movement += orientation*glm::vec3( 0, 0,-r);
	if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS)
		movement += orientation*glm::vec3( 0, 0, r);

	//glm::vec3 orientationEuler = glm::eulerAngles(orientation);
	//std::cout << glm::degrees(orientationEuler.x) << ", " << glm::degrees(orientationEuler.y) << ", " << glm::degrees(orientationEuler.z) << std::endl;
	this->velocity += movement;
	this->velocity *= 0.85;
	glm::vec3 a = this->controlEntity->getPosition();

	playerBounds->center[0] = a.x;
	playerBounds->center[1] = a.y;
	playerBounds->center[2] = a.z;


	float f = 1.f;
	std::vector<glm::vec3> normals;
	this->world->collisionResponse(*playerBounds, &f, &velocity, &normals);
	glm::vec3 step = velocity*(f-0.001f);

	glm::vec3 v = velocity*(1.f-f);
	for(auto &n : normals)
	{
		v -= (n*glm::dot(v, n));
		velocity -= (n*glm::dot(velocity, n));
	}
	step += v;
	
	this->controlEntity->translate(step);
	this->controlEntity->setOrientation(orientation);
}
