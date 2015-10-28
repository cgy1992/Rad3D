/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class GLTexture : public Texture
	{
		DECLARE_RTTI();

		friend class GLHWBufferManager;

	public:
		GLTexture(const String & texName, const String & sourceName);
		virtual ~GLTexture();

		virtual void 
			OnLoad();
		virtual void 
			OnUnload();
		virtual void 
			_loadImp(DataStreamPtr stream);

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();

		virtual bool
			SetColorData(const Float4 & color, int u, int v);
		virtual bool 
			GetColorData(Float4 & color, int u, int v);

		void 
			OnLostDevice();
		void 
			OnResetDevice();

		GLuint 
			GetGLTexture() { return mGLTexture; }

	protected:
		void
			_build();
		void
			_buildNormal();
		void
			_buildCompressed();

		void
			_checkFormat();

	protected:
		GLuint mGLTexture;
		byte * mPixelData;
		int mLockFlag;
	};

}
