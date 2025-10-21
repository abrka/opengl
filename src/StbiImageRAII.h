#pragma once
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

class stbiImage {
public:
	int width;
	int height;
	int nrOfChannels;
	unsigned char* ImageData;

	stbiImage(const std::filesystem::path path) {
		ImageData = stbi_load(path.string().c_str(), &width, &height, &nrOfChannels, 0);
		if (not ImageData) {
			std::cout << "failed to load image data the path was " << path << std::endl;
			assert(false && "failed to load image data");
		}
	}
	~stbiImage() {
		stbi_image_free(ImageData);
	}
	
};