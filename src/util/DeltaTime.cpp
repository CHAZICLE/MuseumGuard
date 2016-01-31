#include "DeltaTime.hpp"

using namespace util;

DeltaTime::DeltaTime(bool vsync, double targetFramerate)
{
	this->targetFramerate = targetFramerate;
	this->currentTime = 0;
	this->deltaTime = 0;
	this->framerate = 0;
	this->vsync = vsync;
}
DeltaTime::~DeltaTime()
{
	
}
void DeltaTime::postTime(double time)
{
	this->deltaTime = this->currentTime-time;
	this->framerate = 1/this->deltaTime;
	this->currentTime = time;
}
double DeltaTime::getTime()
{
	return this->currentTime;
}
double DeltaTime::getTimeDelta()
{
	return this->deltaTime;
}
double DeltaTime::getFramerate()
{
	return this->framerate;
}
double DeltaTime::getTargetFramerate()
{
	return this->targetFramerate;
}
bool DeltaTime::isVsync()
{
	return this->vsync;
}
