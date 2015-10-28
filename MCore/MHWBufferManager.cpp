#include "MHWBufferManager.h"
#include "MRenderHelper.h"

namespace Rad {

	ImplementSingleton(HWBufferManager);

    HWBufferManager::HWBufferManager()
    { 
		mDefaultTextureFilter = eTexFilter::LINEAR;
		mLowFormatTexture = false;
		mLowResolutionTexture = false;
    }

	HWBufferManager::~HWBufferManager()
    {
		d_assert(mTextureMap.Size() == 0);
    }

	void HWBufferManager::SetLowTexture(bool low_format, bool low_resolution)
	{
		mLowFormatTexture = low_format;
		mLowResolutionTexture = low_resolution;
	}

	void HWBufferManager::SetSpecialTexture(const char * name)
	{
		mSpecialTextureSet.Insert(Math::Crc32(name));
	}

	bool HWBufferManager::IsTextureSpecial(const char * name)
	{
		return mSpecialTextureSet.Find(Math::Crc32(name)) != -1;
	}

	TexturePtr HWBufferManager::GetTexture(const String & name)
	{
		return _find(Hash2(name.c_str()), name);
	}

	TexturePtr HWBufferManager::_find(Hash2 hash, const String & name)
	{
		int i = mTextureMap.Find(hash);
		
		d_assert (i == -1 || mTextureMap[i].value->GetName() == name);

		return i != -1 ? mTextureMap[i].value : NULL;
	}

	void HWBufferManager::_delTexture(Texture * p)
	{
		int i = mTextureMap.Find(Hash2(p->GetName().c_str()));

		d_assert (i != -1);

		delete mTextureMap[i].value;
		mTextureMap.Erase(i);
	}

}