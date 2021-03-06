#ifndef FYP_UTIL_QUATERNIONUTILS_HPP_
#define FYP_UTIL_QUATERNIONUTILS_HPP_

#include <glm/glm.hpp>
#include "util/gl.h"

namespace util {
	namespace QuaternionUtils {
		glm::quat rotationBetweenVectors(const glm::vec3 &u0, const glm::vec3 &u1);
		glm::quat rotationBetween(const glm::vec3 &u0, const glm::vec3 &v0, const glm::vec3 &u1, const glm::vec3 &v1);
		glm::vec3 rotate(glm::vec3 v, glm::quat q);
		void calculateQuaternionW(glm::quat &q);
	}
}

#endif
