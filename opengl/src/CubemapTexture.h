#pragma once

#include "glad/glad.h"
#include <iostream>
#include <cassert>
#include <array>

class GlCubemapTexture {


public:
	unsigned int ID = 0;
	GLenum TextureFormat;

;

	GlCubemapTexture(const GLenum _TextureFormat, std::array<unsigned char*, 6> _FacesData, std::array<int, 6>_widths, std::array<int, 6> _heights)
		:TextureFormat(_TextureFormat) {

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

		for (unsigned int i = 0; auto& Face : _FacesData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0 , _TextureFormat, _widths.at(i), _heights.at(i), 0, _TextureFormat, GL_UNSIGNED_BYTE, Face);
			++i;
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}



	void Bind() const {
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}
	void Unbind() const {
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	void Activate(const unsigned int TextureUnit) const {
		glActiveTexture(GL_TEXTURE0 + TextureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}

	GlCubemapTexture(const GlCubemapTexture& rhs) = delete;
	GlCubemapTexture& operator=(const GlCubemapTexture& rhs) = delete;
};