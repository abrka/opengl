#pragma once
#include "Texture.h"
#include "Renderbuffer.h"

class GlFramebuffer {
public:

	unsigned int ID{};

	GlFramebuffer() {
		glGenFramebuffers(1, &ID);
	}

	void AttachTexture(const GlTexture& Tex, const GLenum AttachToWhat, const unsigned int MipmapLevel = 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachToWhat, GL_TEXTURE_2D, Tex.ID, MipmapLevel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void AttachRenderBuffer(const GlRenderBuffer& RenderBuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer.ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Bind() const{
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}

	void Unbind() const{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool CheckStatus() const {
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			return false;
		}
		else {
			return true;
		}
	}
};