#include "GLRenderTarget.h"
#include "GLHWBufferManager.h"
#include "GLUtil.h"

namespace Rad {

	ImplementRTTI(GLRenderTexture, Texture);

	GLRenderTexture::GLRenderTexture()
		: Texture("RenderTexture", "")
	{
		mLoadState = Resource::LOADED;
		mGLTexture = 0;
	}

	GLRenderTexture::~GLRenderTexture()
	{
		d_assert (mGLTexture == 0);
	}

	void GLRenderTexture::Release()
	{
		delete this;
	}

	void * GLRenderTexture::Lock(eLockFlag flag)
	{
		d_assert (0 && "RenderTexture can't lockable");

		return NULL;
	}

	void GLRenderTexture::Unlock()
	{
		d_assert (0 && "RenderTexture can't lockable");
	}

	void GLRenderTexture::_loadImp(DataStreamPtr stream)
	{
		d_assert (0);
	}

	bool GLRenderTexture::SetColorData(const Float4 & color, int u, int v)
	{
		d_assert (0 && "RenderTexture can't call this function");

		return false;
	}

	bool GLRenderTexture::GetColorData(Float4 & color, int u, int v)
	{
		d_assert (0 && "RenderTexture can't call this function");

		return false;
	}


	//
	GLRenderTarget::GLRenderTarget()
		: mGLFrameBuffer(0)
	{
		for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
		{
			mGLTextures[i] = 0;
		}
	}

	GLRenderTarget::~GLRenderTarget()
	{
		OnLostDevice();
	}

	void GLRenderTarget::Release()
	{
		GLHWBufferManager::getGLInstance()->DeleteRenderTarget(this);
	}

	void GLRenderTarget::Resize(int w, int h)
	{
		mWidth = w;
		mHeight = h;

		OnLostDevice();
		OnResetDevice();
	}

	void GLRenderTarget::OnCreate()
	{
		OnResetDevice();
	}

	void GLRenderTarget::OnLostDevice()
	{
		for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
		{
			if (mGLTextures[i] != 0)
			{
				glDeleteTextures(1, &mGLTextures[i]);

				mGLTextures[i] = 0;
			}

			GLRenderTexture * p = (GLRenderTexture *)mTextures[i].c_ptr();
			if (p != NULL)
			{
				p->SetGLTexture(0);
			}
		}

		if (mGLFrameBuffer != 0)
			glDeleteFramebuffers(1, &mGLFrameBuffer);

		mGLFrameBuffer = 0;
	}

	void GLRenderTarget::OnResetDevice()
	{
		d_assert (mGLFrameBuffer == 0);

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

		glGenFramebuffers(1, &mGLFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, mGLFrameBuffer);

		d_assert (glGetError() == 0);

		for (int i = 0; i < MAX_HW_RENDERTARGET; ++i)
		{
			GLRenderTexture * p = (GLRenderTexture *)mTextures[i].c_ptr();

			if (GLUtil::IsRenderTargetFormat(mFormats[i]))
			{
				GLenum _internal = GLUtil::GetGLInernalFormat(mFormats[i]);
				GLenum _format = GLUtil::GetGLPixelFormat(mFormats[i]);
				GLenum _type = GLUtil::GetGLPixelType(mFormats[i]);

				GLuint pGLTexture = 0;
				glGenTextures(1, &pGLTexture);
				glBindTexture(GL_TEXTURE_2D, pGLTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, _internal, mWidth, mHeight, 0, _format, _type, NULL);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pGLTexture, 0);

				p->SetGLSize(mWidth, mHeight);
				p->SetGLTexture(pGLTexture);
			}
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			d_log("Error: create render target '%d' failed.", mFormats[0]);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		d_assert (glGetError() == 0);
	}
}