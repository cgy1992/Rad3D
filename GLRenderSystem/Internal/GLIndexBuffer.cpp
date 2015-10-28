#include "GLIndexBuffer.h"
#include "GLHWBufferManager.h"
#include "GLUtil.h"

namespace Rad {

	GLIndexBuffer::GLIndexBuffer()
		: mIndexData(NULL)
		, mLockFlag(0)
		, mGLBuffer(0)
	{
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		safe_delete_array(mIndexData);

		if (mGLBuffer != 0)
		{
			glDeleteBuffers(1, &mGLBuffer);
			mGLBuffer = 0;
		}
	}

	void GLIndexBuffer::Release()
	{
		GLHWBufferManager::getGLInstance()->DeleteIndexBuffer(this);
	}

	void * GLIndexBuffer::Lock(eLockFlag flag)
	{
		d_assert (mLockFlag == 0);

		mLockFlag = flag;

#ifdef _DEBUG
		if (mLockFlag & eLockFlag::READ)
		{
			d_assert (mUsage == eUsage::STATIC_MANAGED || mUsage == eUsage::DYNAMIC_MANAGED);
		}
#endif

		if (mIndexData == NULL && (mLockFlag & eLockFlag::WRITE))
		{
			mIndexData = new byte[mCount * mStride];
		}

		return mIndexData;
	}

	void GLIndexBuffer::Unlock()
	{
		d_assert (mLockFlag != 0);

		if (mUsage != eUsage::SYSTEM && (mLockFlag & eLockFlag::WRITE))
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * mStride, mIndexData, GLUtil::GetGLUsage(mUsage));

			d_assert (glGetError() == 0);
		}

		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
		{
			safe_delete_array(mIndexData);
		}

		mLockFlag = 0;
	}

	void GLIndexBuffer::OnLostDevice()
	{
		if (mGLBuffer != 0)
		{
			glDeleteBuffers(1, &mGLBuffer);
			mGLBuffer = 0;
		}
	}

	void GLIndexBuffer::OnResetDevice()
	{
		d_assert (mGLBuffer == 0);

		glGenBuffers(1, &mGLBuffer);

		if (mIndexData != NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mGLBuffer);
			glBufferData(GL_ARRAY_BUFFER, mCount * mStride, mIndexData, GLUtil::GetGLUsage(mUsage));
		}
	}

}