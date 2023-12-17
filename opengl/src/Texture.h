#pragma once

#include "glad/glad.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>

class GlTexture {

public:
	unsigned int ID = 0;
	const unsigned int TextureUnit;
	int width;
	int height;
	int nrOfChannles;
	const GLenum TextureFormat;

	GlTexture(const std::string& TexturePath, unsigned int _TextureUnit, GLenum _TextureFormat): TextureUnit(_TextureUnit), TextureFormat(_TextureFormat) {
		
		unsigned char* ImageData = stbi_load(TexturePath.c_str(), &width, &height, &nrOfChannles, 0);

		if (not ImageData) {
			
			std::cout << "failed to load image data" << std::endl;
			assert(false && "failed to load image data");
		}
		
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		

		glTexImage2D(GL_TEXTURE_2D, 0, _TextureFormat, width, height, 0, _TextureFormat, GL_UNSIGNED_BYTE, ImageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(ImageData);
	}

	void Bind() const {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void Activate() const{
		glActiveTexture(GL_TEXTURE0 + TextureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	/*GlTexture(const GlTexture& rhs) = delete;
	GlTexture& operator=(const GlTexture& rhs) = delete;*/
};