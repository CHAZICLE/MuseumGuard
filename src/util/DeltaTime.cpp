#include "DeltaTime.hpp"

using namespace util;

DeltaTime::DeltaTime(bool vsync, double targetFramerate)
{
	this->targetFramerate = targetFramerate;
	this->currentTime = 0;
	this->deltaTime = 0;
	this->framerate = 0;
	this->vsync = vsync;
	this->offsetTime = 0;
}
DeltaTime::~DeltaTime()
{
	
}
void DeltaTime::postTime(double time)
{
	this->deltaTime = time-this->currentTime;
	this->framerate = 1/this->deltaTime;
	this->currentTime = time;
}
void DeltaTime::setOffsetTime(double offsetTime)
{
	this->offsetTime = offsetTime;
}
double DeltaTime::getTime()
{
	return this->currentTime+this->offsetTime;
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
