#pragma once
#include <libraw.h>

struct Image {
	char* data;
	unsigned int textureID;
	std::string name;
	int width;
	int height;
};