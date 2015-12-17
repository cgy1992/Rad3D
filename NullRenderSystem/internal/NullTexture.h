/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTexture.h"

namespace Rad {

	class NullTexture : public Texture
	{
		DECLARE_RTTI();

		friend class NullHWBufferManager;

	public:
		NullTexture(const String & texName, const String & sourceName);
		virtual ~NullTexture();

		virtual void 
			OnLoad();
		virtual void 
			OnUnload();
		virtual void 
			_load(DataStreamPtr stream);

		virtual void * 
			Lock(eLockFlag flag);
		virtual void 
			Unlock();

		virtual bool
			SetColorData(const Float4 & color, int u, int v);
		virtual bool 
			GetColorData(Float4 & color, int u, int v);

	protected:
		byte * mPixelData;
		int mLockFlag;
	};

}
