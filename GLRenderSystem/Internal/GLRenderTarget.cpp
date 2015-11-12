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
		d_assert(0 && "RenderTexture can't call this function");
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
		: mGLTexture(0)
	{
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
		if (mGLTexture != 0)
		{
			glDeleteTextures(1, &mGLTexture);
			mGLTexture = 0;
		}

		GLRenderTexture * p = (GLRenderTexture *)mTexture.c_ptr();

		p->SetGLTexture(0);
	}

	void GLRenderTarget::OnResetDevice()
	{
		d_assert (mWidth > 0 && mHeight > 0 && mGLTexture == 0);

		GLRenderTexture * p = (GLRenderTexture *)mTexture.c_ptr();

		if (GLUtil::IsRenderTargetFormat(mFormat))
		{
			GLenum _internal = GLUtil::GetGLInernalFormat(mFormat);
			GLenum _format = GLUtil::GetGLPixelFormat(mFormat);
			GLenum _type = GLUtil::GetGLPixelType(mFormat);

			glGenTextures(1, &mGLTexture);
			glBindTexture(GL_TEXTURE_2D, mGLTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, _internal, mWidth, mHeight, 0, _format, _type, NULL);

			p->SetGLSize(mWidth, mHeight);
			p->SetGLTexture(mGLTexture);
		}

		p->_update();

		d_assert (glGetError() == 0);
	}

}