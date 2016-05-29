#ifndef __STREAMUTILS_H_INCLUDED__
#define __STREAMUTILS_H_INCLUDED__

#include <string>
#include <istream>

//#define ENABLE_STREAMUTILS_DEBUG

#define readVec3f(x) glm::vec3(readFloat(x), readFloat(x), readFloat(x))

namespace util {
	namespace StreamUtils {

#ifdef ENABLE_STREAMUTILS_DEBUG
		std::string readString(const char *file, int line, std::istream &fp);
		int readInt(const char *file, int line, std::istream &fp);
		float readFloat(const char *file, int line, std::istream &fp);
		bool readBool(const char *file, int line, std::istream &fp);
#else
		std::string readString(std::istream &fp);
		int readInt(std::istream &fp);
		float readFloat(std::istream &fp);
		bool readBool(std::istream &fp);
#endif
	}
}

#ifndef __STRSKIP
#ifdef ENABLE_STREAMUTILS_DEBUG

#define readString(fp) util::StreamUtils::readString(__FILE__, __LINE__, fp)
#define readInt(fp) util::StreamUtils::readInt(__FILE__, __LINE__, fp)
#define readFloat(fp) util::StreamUtils::readFloat(__FILE__, __LINE__, fp)
#define readBool(fp) util::StreamUtils::readBool(__FILE__, __LINE__, fp)

#endif
#endif

#endif
