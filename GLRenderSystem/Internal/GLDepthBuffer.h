/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	class GLHWBufferManager;

	class GLDepthBuffer : public DepthBuffer
	{
		friend class GLHWBufferManager;

		DECLARE_LINKER(GLDepthBuffer);

	public:
		GLDepthBuffer();
		virtual ~GLDepthBuffer();

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
			GetGLDepthBuffer() { return mGLDepthBuffer; }

	protected:
		GLuint mGLDepthBuffer;
	};
	

}