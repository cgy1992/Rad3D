/*
*	HWBufferManager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSet.h"
#include "MMap.h"
#include "MSingleton.h"
#include "MVertexBuffer.h"
#include "MIndexBuffer.h"
#include "MTexture.h"
#include "MRenderTarget.h"
#include "MDepthBuffer.h"

namespace Rad {

	class M_ENTRY HWBufferManager : public Singleton<HWBufferManager>
	{
	public:
		HWBufferManager();
		virtual ~HWBufferManager();

		virtual VertexBufferPtr 
			NewVertexBuffer(int stride, int count, eUsage usage = eUsage::STATIC_MANAGED) = 0;

		virtual IndexBufferPtr 
			NewIndexBuffer(int count, eUsage usage = eUsage::STATIC_MANAGED) = 0;

		void 
			SetDefaultTextureFilter(eTexFilter type);
		eTexFilter 
			GetDefaultTextureFilter() { return mDefaultTextureFilter; }

		void
			SetLowTexture(bool low_format, bool low_resolution);
		bool
			IsTextureLowFormat() { return mLowFormatTexture; }
		bool
			IsTextureLowResolution() { return mLowResolutionTexture; }

		void
			SetSpecialTexture(const char * name);
		bool
			IsTextureSpecial(const char * name);

		virtual TexturePtr 
			NewTexture(const String & name, int w, int h, int mipmaps = -1, 
				ePixelFormat format = ePixelFormat::R8G8B8A8, eUsage usage = eUsage::STATIC_MANAGED) = 0;
		virtual TexturePtr 
			LoadTexture(const String & filename, float priority = 0, int mipmaps = -1, eUsage usage = eUsage::STATIC_MANAGED) = 0;
		TexturePtr 
			GetTexture(const String & name);
		void 
			_delTexture(Texture * p);

		virtual void
			RebuildAllTexture() = 0;

		virtual RenderTargetPtr 
			NewRenderTarget(int width, int height, eSizeAlign align = eSizeAlign::NONE, ePixelFormat format = ePixelFormat::R8G8B8) = 0;
		virtual RenderTargetPtr
			NewMultiRenderTarget(int width, int height, eSizeAlign align, ePixelFormat * formats, int count) = 0;
		virtual DepthBufferPtr
			NewDepthBuffer(int width, int height, eSizeAlign align = eSizeAlign::NONE, ePixelFormat format = ePixelFormat::D16) = 0;

	protected:
		TexturePtr 
			_find(Hash2 hash, const String & name);
		
	protected:
		eTexFilter mDefaultTextureFilter;
		bool mLowFormatTexture;
		bool mLowResolutionTexture;
		Set<int> mSpecialTextureSet;

		Map<Hash2, Texture *> mTextureMap;
	};

}
