#pragma once

#include "glad/glad.h"
#include <iostream>
#include <cassert>

struct TextureSpec {
	bool GenerateMipmap{ true };
	GLenum WrapMode{ GL_REPEAT };
	GLenum TextureDataType{ GL_UNSIGNED_BYTE };
};


class GlTexture {

public:
	unsigned int ID = 0;
	unsigned int width;
	unsigned int height;
	GLenum TextureFormat;

	GlTexture(const GLenum _InternalTexFormat, const GLenum _TextureFormat, unsigned int _width, unsigned int _height, unsigned char* _TextureData, TextureSpec _TexSpec = TextureSpec{})
		: width(_width), height(_height), TextureFormat(_TextureFormat) {

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _TexSpec.WrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _TexSpec.WrapMode);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, _InternalTexFormat, _width, _height, 0, _TextureFormat, _TexSpec.TextureDataType, _TextureData);

		if (_TexSpec.GenerateMipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	
	}



	void Bind() const {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Activate(const unsigned int TextureUnit) const {
		glActiveTexture(GL_TEXTURE0 + TextureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	GlTexture(const GlTexture& rhs) = delete;
	GlTexture& operator=(const GlTexture& rhs) = delete;
};