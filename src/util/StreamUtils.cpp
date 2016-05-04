#include "StreamUtils.hpp"

using namespace util;

std::string StreamUtils::readString(std::istream &fp)
{
	int len;
	fp.read((char *)&len, sizeof(int));
	char *buf = new char[len+1];
	fp.read(buf, len);
	buf[len] = '\0';
	return std::string(buf);
}
int StreamUtils::readInt(std::istream &fp)
{
	int i;
	fp.read((char *)&i, sizeof(int));
	return i;
}
float StreamUtils::readFloat(std::istream &fp)
{
	float i;
	fp.read((char *)&i, sizeof(float));
	return i;
}
bool StreamUtils::readBool(std::istream &fp)
{
	bool i;
	fp.read((char *)&i, sizeof(bool));
	return i;
}
