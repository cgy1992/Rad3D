#include "NullHWBufferManager.h"

namespace Rad {

	NullHWBufferManager::NullHWBufferManager()
	{
	}

	NullHWBufferManager::~NullHWBufferManager()
	{
	}

	VertexBufferPtr NullHWBufferManager::NewVertexBuffer(
		int stride, int count, eUsage usage)
	{
		NullVertexBuffer * p = new NullVertexBuffer;
		p->mStride = stride;
		p->mCount = count;
		p->mUsage = usage;

		return p;
	}

	IndexBufferPtr NullHWBufferManager::NewIndexBuffer(
		int count, eUsage usage)
	{
		int stride = count < 65536 ? 2 : 4;

		NullIndexBuffer * p = new NullIndexBuffer;
		p->mCount = count;
		p->mStride = stride;
		p->mUsage = usage;

		return p;
	}

	TexturePtr NullHWBufferManager::NewTexture(const String & name, int width, int height, int mipmaps, ePixelFormat format, eUsage usage)
	{
		d_assert (GetTexture(name) == NULL);

		NullTexture * p = new NullTexture(name, "");
		p->mWidth = width;
		p->mHeight = height;
		p->mMipmaps = 1;
		p->mUsage = usage;
		p->mFormat = format;
		p->Load();

		mTextureMap.Insert(Hash2(name.c_str()), p);

		return p;
	}

	TexturePtr NullHWBufferManager::LoadTexture(const String & filename, float priority, int mipmaps, eUsage usage)
	{
		Hash2 hash(filename.c_str());

		TexturePtr p = _find(hash, filename);
		if (p == NULL)
		{
			NullTexture * pTexture = new NullTexture(filename, filename);
			pTexture->mWidth = 0;
			pTexture->mHeight = 0;
			pTexture->mMipmaps = 1;
			pTexture->mUsage = usage;
			pTexture->mFormat = ePixelFormat::UNKNOWN;
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

	RenderTargetPtr NullHWBufferManager::NewRenderTarget(int width, int height, eSizeAlign align, ePixelFormat format)
	{
		return NewMultiRenderTarget(width, height, align, &format, 1);
	}

	RenderTargetPtr NullHWBufferManager::NewMultiRenderTarget(int width, int height, eSizeAlign align, ePixelFormat * formats, int count)
	{
		NullRenderTarget * p = new NullRenderTarget;
		p->mWidth = width;
		p->mHeight = height;
		p->mAlign = align;

		return p;
	}

	DepthBufferPtr NullHWBufferManager::NewDepthBuffer(int width, int height, eSizeAlign align, ePixelFormat format)
	{
		NullDepthBuffer * p = new NullDepthBuffer;
		p->mWidth = width;
		p->mHeight = height;
		p->mAlign = align;
		p->mFormat = format;

		return p;
	}

}

