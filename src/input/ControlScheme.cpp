#include "util/DeltaTime.hpp"
#include "input/controls/DebugControls.hpp"

#include "ControlScheme.hpp"

using namespace controls;
using namespace util;

ControlScheme::ControlScheme(Entity *controlEntity)
{
	this->controlEntity = controlEntity;
}
ControlScheme::~ControlScheme()
{
	
}
void ControlScheme::tick(DeltaTime &deltaTime)
{
	
}
