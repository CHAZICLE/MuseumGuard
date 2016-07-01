#include "util/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/norm.hpp>

#include "QuaternionUtils.hpp"

using namespace util;
using namespace glm;

//CITE: http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors - 22nd June 2016
quat util::QuaternionUtils::rotationBetweenVectors(const vec3 &u, const vec3 &v)
{
	vec3 w = cross(u, v);
	quat q = quat(dot(u, v), w.x, w.y, w.z);
	q.w += length(q);
	return normalize(q);
}
quat util::QuaternionUtils::rotationBetween(const vec3 &v0, const vec3 &u0, const vec3 &v2, const vec3 &u2)
{
	//up,forward = 
	quat q2 = rotationBetweenVectors(u0, u2);

	vec3 v1 = rotate(v2, q2);
	vec3 v0_proj = proj(v0, u0);
	vec3 v1_proj = proj(v1, u0);
	quat q1 = rotationBetweenVectors(v0_proj, v1_proj);

	return normalize(q2*q1);
}
vec3 util::QuaternionUtils::rotate(vec3 v, quat q)
{
	glm::vec3 qv = glm::vec3(q.x, q.y, q.z);
	glm::vec3 t = 2.f * cross(glm::vec3(q.x, q.y, q.z), v);
	return v + q.w * t + cross(qv, t);
}
