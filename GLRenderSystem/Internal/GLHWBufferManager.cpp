#include "GLHWBufferManager.h"
#include "GLUtil.h"

namespace Rad {

	GLHWBufferManager::GLHWBufferManager()
		: mVertexBufferLinker(NULL)
		, mIndexBufferLinker(NULL)
		, mRenderTargetLinker(NULL)
		, mDepthBufferLinker(NULL)
	{
		glGenFramebuffers(1, &mFrameBuffer);
	}

	GLHWBufferManager::~GLHWBufferManager()
	{
		glDeleteFramebuffers(1, &mFrameBuffer);

		d_assert (mVertexBufferLinker == NULL);
		d_assert (mIndexBufferLinker == NULL);
		d_assert (mRenderTargetLinker == NULL);
		d_assert (mDepthBufferLinker == NULL);
	}

	VertexBufferPtr GLHWBufferManager::NewVertexBuffer(int stride, int count, eUsage usage)
	{
		GLuint buffer = 0;

		glGenBuffers(1, &buffer);

		GLVertexBuffer * p = new GLVertexBuffer;

		p->mGLBuffer = buffer;
		p->mStride = stride;
		p->mCount = count;
		p->mUsage = usage;

		LINKER_APPEND(mVertexBufferLinker, p);

		return p;
	}

	IndexBufferPtr GLHWBufferManager::NewIndexBuffer(int count, eUsage usage)
	{
		int stride = count < 65536 ? 2 : 4;

		GLuint buffer;

		glGenBuffers(1, &buffer);

		GLIndexBuffer * p = new GLIndexBuffer;

		p->mGLBuffer = buffer;
		p->mCount = count;
		p->mStride = stride;
		p->mUsage = usage;

		LINKER_APPEND(mIndexBufferLinker, p);

		return p;
	}

	void GLHWBufferManager::DeleteVertexBuffer(GLVertexBuffer * vb)
	{
		LINKER_REMOVE(mVertexBufferLinker, vb);
		delete vb;
	}

	void GLHWBufferManager::DeleteIndexBuffer(GLIndexBuffer * ib)
	{
		LINKER_REMOVE(mIndexBufferLinker, ib);
		delete ib;
	}

	TexturePtr GLHWBufferManager::NewTexture(const String & name, int width, int height, int mipmaps, ePixelFormat format, eUsage usage)
	{
		d_assert (GetTexture(name) == NULL);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLTexture * p = new GLTexture(name, "");
		p->mWidth = width;
		p->mHeight = height;
		p->mMipmaps = mipmaps;
		p->mUsage = usage;
		p->mFormat = format;
		p->mGLTexture = texture;
		p->Load();

		mTextureMap.Insert(Hash2(name.c_str()), p);

		return p;
	}

	TexturePtr GLHWBufferManager::LoadTexture(const String & filename, float priority, int mipmaps, eUsage usage)
	{
		Hash2 hash(filename.c_str());

		TexturePtr p = _find(hash, filename);
		if (p == NULL)
		{
			GLTexture * pTexture = new GLTexture(filename, filename);
			pTexture->mWidth = 0;
			pTexture->mHeight = 0;
			pTexture->mMipmaps = mipmaps;
			pTexture->mUsage = usage;
			pTexture->mFormat = ePixelFormat::UNKNOWN;
			pTexture->mGLTexture = 0;
			pTexture->mPriority = priority;

			p = pTexture;

			mTextureMap.Insert(hash, p.c_ptr());
		}

		if (priority < 0)
		{
			p->EnsureLoad();
		}
		else
		{
			p->Load();
		}

		return p;
	}

	void GLHWBufferManager::RebuildAllTexture()
	{
		for (int i = 0; i < mTextureMap.Size(); ++i)
		{
			GLTexture * pGLTexture = (GLTexture *)mTextureMap[i].value;

			if (pGLTexture->GetUsage() != eUsage::SYSTEM)
			{
				pGLTexture->OnLostDevice();
				pGLTexture->OnResetDevice();
			}
		}
	}

	RenderTargetPtr GLHWBufferManager::NewRenderTarget(int width, int height, ePixelFormat format)
	{
		d_assert (GLUtil::IsRenderTargetFormat(format));

		GLRenderTarget * p = new GLRenderTarget;
		p->mWidth = width;
		p->mHeight = height;
		p->mFormat = format;

		GLRenderTexture * t = new GLRenderTexture;
		t->mWidth = width;
		t->mHeight = height;
		t->mMipmaps = 1;
		t->mFormat = format;
		t->mUsage = eUsage::DYNAMIC;

		p->mTexture = t;
		p->OnCreate();

		LINKER_APPEND(mRenderTargetLinker, p);

		return p;
	}

	void GLHWBufferManager::DeleteRenderTarget(GLRenderTarget * p)
	{
		LINKER_REMOVE(mRenderTargetLinker, p);

		delete p;
	}

	DepthBufferPtr GLHWBufferManager::NewDepthBuffer(int width, int height, ePixelFormat format)
	{
		GLDepthBuffer * p = new GLDepthBuffer;
		p->mWidth = width;
		p->mHeight = height;
		p->mFormat = format;

		p->OnCreate();

		LINKER_APPEND(mDepthBufferLinker, p);

		return p;
	}

	void GLHWBufferManager::DeleteDepthBuffer(GLDepthBuffer * p)
	{
		LINKER_REMOVE(mDepthBufferLinker, p);

		delete p;
	}

	void GLHWBufferManager::OnLostDevice()
	{
		glGenFramebuffers(1, &mFrameBuffer);

		GLVertexBuffer * vb = mVertexBufferLinker;
		while (vb != NULL)
		{
			if (vb->GetUsage() != eUsage::SYSTEM)
			{
				vb->OnLostDevice();
			}

			vb = LINKER_NEXT(vb);
		}

		GLIndexBuffer * ib = mIndexBufferLinker;
		while (ib != NULL)
		{
			if (ib->GetUsage() != eUsage::SYSTEM)
			{
				ib->OnLostDevice();
			}

			ib = LINKER_NEXT(ib);
		}

		for (int i = 0; i < mTextureMap.Size(); ++i)
		{
			GLTexture * pGLTexture = (GLTexture *)mTextureMap[i].value;

			if (pGLTexture->GetUsage() != eUsage::SYSTEM)
			{
				pGLTexture->OnLostDevice();
			}
		}

		GLRenderTarget * pRenderTarget = mRenderTargetLinker;
		while (pRenderTarget != NULL)
		{
			pRenderTarget->OnLostDevice();

			pRenderTarget = LINKER_NEXT(pRenderTarget);
		}

		GLDepthBuffer * pDepthBuffer = mDepthBufferLinker;
		while (pDepthBuffer != NULL)
		{
			pDepthBuffer->OnLostDevice();

			pDepthBuffer = LINKER_NEXT(pDepthBuffer);
		}
	}

	void GLHWBufferManager::OnResetDevice()
	{
		GLVertexBuffer * vb = mVertexBufferLinker;
		while (vb != NULL)
		{
			if (vb->GetUsage() != eUsage::SYSTEM)
			{
				vb->OnResetDevice();
			}

			vb = LINKER_NEXT(vb);
		}

		GLIndexBuffer * ib = mIndexBufferLinker;
		while (ib != NULL)
		{
			if (ib->GetUsage() != eUsage::SYSTEM)
			{
				ib->OnResetDevice();
			}

			ib = LINKER_NEXT(ib);
		}

		for (int i = 0; i < mTextureMap.Size(); ++i)
		{
			GLTexture * pGLTexture = (GLTexture *)mTextureMap[i].value;

			if (pGLTexture->GetUsage() != eUsage::SYSTEM)
			{
				pGLTexture->OnResetDevice();
			}
		}

		GLRenderTarget * pRenderTarget = mRenderTargetLinker;
		while (pRenderTarget != NULL)
		{
			pRenderTarget->OnResetDevice();

			pRenderTarget = LINKER_NEXT(pRenderTarget);
		}

		GLDepthBuffer * pDepthBuffer = mDepthBufferLinker;
		while (pDepthBuffer != NULL)
		{
			pDepthBuffer->OnResetDevice();

			pDepthBuffer = LINKER_NEXT(pDepthBuffer);
		}

		glDeleteFramebuffers(1, &mFrameBuffer);
		mFrameBuffer = 0;
	}

}

