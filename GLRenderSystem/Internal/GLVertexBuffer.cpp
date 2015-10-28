#include "GLVertexBuffer.h"
#include "GLHWBufferManager.h"
#include "GLUtil.h"

namespace Rad {

	GLVertexBuffer::GLVertexBuffer()
		: mVertexData(NULL)
		, mLockFlag(0)
		, mGLBuffer(0)
	{
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		safe_delete_array(mVertexData);
		
		if (mGLBuffer != 0)
		{
			glDeleteBuffers(1, &mGLBuffer);
			mGLBuffer = 0;
		}
	}

	void GLVertexBuffer::Release()
	{
		GLHWBufferManager::getGLInstance()->DeleteVertexBuffer(this);
	}

	void * GLVertexBuffer::Lock(eLockFlag flag)
	{
		d_assert(mLockFlag == 0);

		mLockFlag = flag;

#ifdef _DEBUG
		if (mLockFlag & eLockFlag::READ)
		{
			d_assert (mUsage == eUsage::STATIC_MANAGED || mUsage == eUsage::DYNAMIC_MANAGED);
		}
#endif

		if (mVertexData == NULL && (mLockFlag & eLockFlag::WRITE))
		{
			mVertexData = new byte[mStride * mCount];
		}

		d_assert(glGetError() == 0);

		return mVertexData;
	}

	void GLVertexBuffer::Unlock()
	{
		d_assert(mLockFlag != 0);

		if (mUsage != eUsage::SYSTEM && (mLockFlag & eLockFlag::WRITE) && mVertexData != NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mGLBuffer);
			glBufferData(GL_ARRAY_BUFFER, mStride * mCount, mVertexData, GLUtil::GetGLUsage(mUsage));
		}
		
		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
			safe_delete_array(mVertexData);

		d_assert(glGetError() == 0);

		mLockFlag = 0;
	}

	void GLVertexBuffer::OnLostDevice()
	{
		if (mGLBuffer != 0)
		{
			glDeleteBuffers(1, &mGLBuffer);
			mGLBuffer = 0;
		}
	}

	void GLVertexBuffer::OnResetDevice()
	{
		d_assert(mGLBuffer == 0);

		glGenBuffers(1, &mGLBuffer);

		if (mVertexData != NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mGLBuffer);
			glBufferData(GL_ARRAY_BUFFER, mStride * mCount, mVertexData, GLUtil::GetGLUsage(mUsage));
		}
	}

}