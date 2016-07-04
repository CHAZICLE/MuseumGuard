#ifndef __SUPERDEBUG_H_INCLUDED__
#define __SUPERDEBUG_H_INCLUDED__
#ifndef DISABLE_DEBUG

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define _IOSTREAM_HEADER "DEBUG:[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "] "

#define PRINT_CONTROL(message, control, action) PRINT_DEBUG(message << " (Control:" << control << ", Action:" << action << ")")

#define PRINT_DEBUG(msg) std::cout << _IOSTREAM_HEADER << msg << std::endl;

inline std::ostream &operator<<(std::ostream &ost, const glm::vec3 &a)
{
	return ost << "(" << a.x << ", " << a.y << ", " << a.z << ")";
}

inline std::ostream &operator<<(std::ostream &ost, const glm::quat &a)
{
#ifdef DEBUG_QUAT
	return ost << "(Q:" << a.x << ", " << a.y << ", " << a.z << " : " << a.w << "; Euler:" << glm::degrees(glm::eulerAngles(a)) << ")";
#else
	return ost << "(Euler:" << glm::degrees(glm::eulerAngles(a)) << ")";
#endif
}

#endif
#endif
