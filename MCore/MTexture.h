/*
*	Texture
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MTypes.h"
#include "MResource.h"

namespace Rad {

	class M_ENTRY Texture : public Resource
	{
		DECLARE_RTTI_ROOT(Texture);

	public:
		Texture(const String & texName, const String & sourceName);
		virtual ~Texture();

		virtual void 
			Release();

		virtual void * 
			Lock(eLockFlag flag) = 0;
		virtual void 
			Unlock() = 0;

		virtual bool
			SetColorData(const Float4 & color, int u, int v) = 0;
		virtual bool 
			GetColorData(Float4 & color, int u, int v) = 0;
		
		void
			SampleColor(Float4 & color, float fu, float fv);
		void
			StrechBlt(Texture * dest, const RectI * rcDest, const RectI * rcSrc);

		int 
			GetWidth() const { return mWidth; }
		int 
			GetHeight() const { return mHeight; }
		int 
			GetMipmaps() const { return mMipmaps; }
		int
			GetMipmapLevel() const { return mMipmapLevel; }
		eUsage 
			GetUsage() const { return mUsage; }
		ePixelFormat 
			GetFormat() const { return mFormat; }

		void
			_update();
		float
			_getInvWidth() { return i_inv_width; }
		float
			_getInvHeight() { return i_inv_height; }
		float
			_getAscept() { return i_ascept; }

	protected:
		int mWidth;
		int mHeight;
		int mMipmaps;
		int mMipmapLevel;
		eUsage mUsage;
		ePixelFormat mFormat;

		float i_inv_width;
		float i_inv_height;
		float i_ascept;
	};

	typedef SmartPtr<Texture> TexturePtr;
}
