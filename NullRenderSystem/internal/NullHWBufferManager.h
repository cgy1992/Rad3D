/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MHWBufferManager.h"
#include "NullVertexBuffer.h"
#include "NullIndexBuffer.h"
#include "NullTexture.h"
#include "NullRenderTarget.h"
#include "NullDepthBuffer.h"

namespace Rad {

	class NullHWBufferManager : public HWBufferManager
	{
	public:
		NullHWBufferManager();
		virtual ~NullHWBufferManager();

		//
		virtual VertexBufferPtr 
			NewVertexBuffer(int stride, int count, eUsage usage);

		virtual IndexBufferPtr 
			NewIndexBuffer(int count, eUsage usage);

		//
		virtual TexturePtr 
			NewTexture(const String & name, int w, int h, int mipmaps, ePixelFormat format, eUsage usage);
		virtual TexturePtr 
			LoadTexture(const String & filename, float priority, int mipmaps, eUsage usage);
		virtual void
			RebuildAllTexture() {}

		virtual RenderTargetPtr 
			NewRenderTarget(int width, int height, ePixelFormat format);
		virtual DepthBufferPtr
			NewDepthBuffer(int width, int height, ePixelFormat format);
	};

}
