/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLRenderTarget.h"
#include "GLDepthBuffer.h"

namespace Rad {

	class GLHWBufferManager : public HWBufferManager
	{
	public:
		static GLHWBufferManager * 
			getGLInstance() { return (GLHWBufferManager *)Instance(); }

	public:
		GLHWBufferManager();
		virtual ~GLHWBufferManager();

		//
		virtual VertexBufferPtr 
			NewVertexBuffer(int stride, int count, eUsage usage);
		void 
			DeleteVertexBuffer(GLVertexBuffer * vb);

		virtual IndexBufferPtr 
			NewIndexBuffer(int count, eUsage usage);
		void 
			DeleteIndexBuffer(GLIndexBuffer * ib);

		//
		virtual TexturePtr 
			NewTexture(const String & name, int w, int h, int mipmaps, ePixelFormat format, eUsage usage);
		virtual TexturePtr 
			LoadTexture(const String & filename, float priority, int mipmaps, eUsage usage);
		virtual void
			RebuildAllTexture();

		virtual RenderTargetPtr 
			NewRenderTarget(int width, int height, ePixelFormat format);
		void 
			DeleteRenderTarget(GLRenderTarget * p);

		virtual DepthBufferPtr
			NewDepthBuffer(int width, int height, ePixelFormat format);
		void
			DeleteDepthBuffer(GLDepthBuffer * p);

		void 
			OnLostDevice();
		void 
			OnResetDevice();

		GLuint
			GetFrameBuffer() { return mFrameBuffer; }

	protected:
		GLVertexBuffer * mVertexBufferLinker;
		GLIndexBuffer * mIndexBufferLinker;

		GLRenderTarget * mRenderTargetLinker;
		GLDepthBuffer * mDepthBufferLinker;

		GLuint mFrameBuffer;
	};

}
