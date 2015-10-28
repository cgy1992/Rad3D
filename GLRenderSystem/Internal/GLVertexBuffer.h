/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class GLVertexBuffer : public VertexBuffer
	{
		friend class GLHWBufferManager;

		DECLARE_LINKER(GLVertexBuffer);

	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();

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

		byte * mVertexData;
		int mLockFlag;
	};

}
