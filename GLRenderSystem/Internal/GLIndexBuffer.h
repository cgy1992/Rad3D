/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class GLIndexBuffer : public IndexBuffer
	{
		friend class GLHWBufferManager;

		DECLARE_LINKER(GLIndexBuffer);

	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		virtual void 
			Release();

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();

		void 
			OnLostDevice();
		void 
			OnResetDevice();

		GLuint 
			GetGLBuffer() { return mGLBuffer; } 

	protected:
		GLuint mGLBuffer;

		byte * mIndexData;
		int mLockFlag;
	};

}
