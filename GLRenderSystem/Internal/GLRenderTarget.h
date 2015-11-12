/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class GLHWBufferManager;

	class GLRenderTexture : public Texture
	{
		DECLARE_RTTI();

		friend class GLHWBufferManager;

	public:
		GLRenderTexture();
		virtual ~GLRenderTexture();

		virtual void
			Release() { delete this; }

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();
		virtual void 
			_loadImp(DataStreamPtr stream);

		virtual bool
			SetColorData(const Float4 & color, int u, int v);
		virtual bool 
			GetColorData(Float4 & color, int u, int v);

		void 
			SetGLTexture(GLuint p) { mGLTexture = p; }
		GLuint 
			GetGLTexture() { return mGLTexture; }

		void
			SetGLSize(int w, int h) { mWidth = w, mHeight = h; }

	protected:
		GLuint mGLTexture;
	};


	class GLRenderTarget : RenderTarget
	{
		friend class GLHWBufferManager;

		DECLARE_LINKER(GLRenderTarget);

	public:
		GLRenderTarget();
		virtual ~GLRenderTarget();

		virtual void
			Release();

		virtual void
			Resize(int w, int h);

		void
			OnCreate();
		void 
			OnLostDevice();
		void 
			OnResetDevice();

		GLuint 
			GetGLTexture() { return mGLTexture; }

	protected:
		GLuint mGLTexture;
	};

}