#ifndef __STREAMUTILS_H_INCLUDED__
#define __STREAMUTILS_H_INCLUDED__

#include <string>
#include <istream>

#define readVec3f(x) glm::vec3(readFloat(x), readFloat(x), readFloat(x))

namespace util {
	namespace StreamUtils {
		std::string readString(std::istream &fp);
		int readInt(std::istream &fp);
		float readFloat(std::istream &fp);
		bool readBool(std::istream &fp);
	}
}

#endif
