#pragma once
#include "glad/glad.h"

class GlRenderBuffer {
public:
	unsigned int ID{};

	GlRenderBuffer(GLenum InternalFormat, unsigned int width, unsigned int height) {
		glGenRenderbuffers(1, &ID);
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
		glRenderbufferStorage(GL_RENDERBUFFER, InternalFormat, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0); // 0
	}
	void Bind() const{
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
	}
	void Unbind() const {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
};