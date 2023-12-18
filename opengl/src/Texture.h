#pragma once

#include "glad/glad.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

class GlTexture {

public:
	unsigned int ID = 0;
	
	int width;
	int height;
	int nrOfChannels;
	GLenum TextureFormat;

	GlTexture(const std::filesystem::path TexturePath) {
		
		std::cout << TexturePath << std::endl;
		unsigned char* ImageData = stbi_load(TexturePath.string().c_str(), &width, &height, &nrOfChannels, 0);

		if (not ImageData) {
			
			std::cout << "failed to load image data the path was "<< TexturePath<< std::endl;
			assert(false && "failed to load image data" );
		}
		
		GLenum _TextureFormat{};
		switch (nrOfChannels)
		{
		case 3:
			_TextureFormat = GL_RGB;
			break;
		case 4:
			_TextureFormat = GL_RGBA;
			break;
		default:
			assert(false && "number of image channels not supported");
			break;
		}
		TextureFormat = _TextureFormat;


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
	void Activate(const unsigned int TextureUnit) const {
		glActiveTexture(GL_TEXTURE0 + TextureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	GlTexture(const GlTexture& rhs) = delete;
	GlTexture& operator=(const GlTexture& rhs) = delete;
};