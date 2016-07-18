#define __STRSKIP
#include <iostream>
#include "StreamUtils.hpp"

using namespace util;

#ifdef ENABLE_STREAMUTILS_DEBUG
std::string StreamUtils::readString(const char *file, int line, std::istream &fp)
#else
std::string StreamUtils::readString(std::istream &fp)
#endif
{
	int len;
	fp.read((char *)&len, sizeof(int));
	char *buf = new char[len+1];
	fp.read(buf, len);
	buf[len] = '\0';
#ifdef ENABLE_STREAMUTILS_DEBUG
	std::cout << "[" << file << ":" << line << "]readString(fp): " << buf << std::endl;
#endif
	std::string s = std::string(buf);
	delete [] buf;
	return s;
}
#ifdef ENABLE_STREAMUTILS_DEBUG
int StreamUtils::readInt(const char *file, int line, std::istream &fp)
#else
int StreamUtils::readInt(std::istream &fp)
#endif
{
	int i;
	fp.read((char *)&i, sizeof(int));
#ifdef ENABLE_STREAMUTILS_DEBUG
	std::cout << "[" << file << ":" << line << "]readInt(fp): " << i << std::endl;
#endif
	return i;
}
#ifdef ENABLE_STREAMUTILS_DEBUG
float StreamUtils::readFloat(const char *file, int line, std::istream &fp)
#else
float StreamUtils::readFloat(std::istream &fp)
#endif
{
	float i;
	fp.read((char *)&i, sizeof(float));
#ifdef ENABLE_STREAMUTILS_DEBUG
	std::cout << "[" << file << ":" << line << "]readFloat(fp): " << i << std::endl;
#endif
	return i;
}
#ifdef ENABLE_STREAMUTILS_DEBUG
bool StreamUtils::readBool(const char *file, int line, std::istream &fp)
#else
bool StreamUtils::readBool(std::istream &fp)
#endif
{
	bool i;
	fp.read((char *)&i, sizeof(bool));
#ifdef ENABLE_STREAMUTILS_DEBUG
	std::cout << "[" << file << ":" << line << "]readBool(fp): " << i << std::endl;
#endif
	return i;

}
