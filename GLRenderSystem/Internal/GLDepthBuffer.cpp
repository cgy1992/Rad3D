#include "GLDepthBuffer.h"
#include "GLHWBufferManager.h"
#include "GLUtil.h"

namespace Rad {


	GLDepthBuffer::GLDepthBuffer()
		: mGLDepthBuffer(0)
	{
	}

	GLDepthBuffer::~GLDepthBuffer()
	{
		OnLostDevice();
	}

	void GLDepthBuffer::Release()
	{
		GLHWBufferManager::getGLInstance()->DeleteDepthBuffer(this);
	}

	void GLDepthBuffer::OnCreate()
	{
		OnResetDevice();
	}

	void GLDepthBuffer::Resize(int w, int h)
	{
		mWidth = w;
		mHeight = h;

		OnLostDevice();
		OnResetDevice();
	}

	void GLDepthBuffer::OnLostDevice()
	{
		if (mGLDepthBuffer != 0)
			glDeleteRenderbuffers(1, &mGLDepthBuffer);
	}

	void GLDepthBuffer::OnResetDevice()
	{
		if (mAlign == eSizeAlign::FULL)
		{
			mWidth = RenderSystem::Instance()->GetConfig().width;
			mHeight = RenderSystem::Instance()->GetConfig().height;
		}
		else if (mAlign == eSizeAlign::HALF)
		{
			mWidth = RenderSystem::Instance()->GetConfig().width / 2;
			mHeight = RenderSystem::Instance()->GetConfig().height / 2;
		}
		else if (mAlign == eSizeAlign::QUAD)
		{
			mWidth = RenderSystem::Instance()->GetConfig().width / 4;
			mHeight = RenderSystem::Instance()->GetConfig().height / 4;
		}

		d_assert (mWidth > 0 && mHeight > 0);

		GLuint fbo = GLHWBufferManager::getGLInstance()->GetTempFbo();
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenRenderbuffers(1, &mGLDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mGLDepthBuffer);

		switch (mFormat)
		{
		case ePixelFormat::D16:
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mWidth, mHeight);
			break;

		case ePixelFormat::D24:
#if defined(GL_DEPTH_COMPONENT24)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mWidth, mHeight);
#elif defined(GL_DEPTH24_STENCIL8_OES)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, mWidth, mHeight);
#else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mWidth, mHeight);
#endif
			break;

		case ePixelFormat::D24S8:
#if defined(GL_DEPTH24_STENCIL8)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
#elif defined(GL_DEPTH24_STENCIL8_OES)
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, mWidth, mHeight);
#else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mWidth, mHeight);
#endif
			break;
		}

		d_assert (glGetError() == 0);
	}

}