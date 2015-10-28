#include "MTexture.h"
#include "MHWBufferManager.h"
#include "MResourceManager.h"

namespace Rad {

	ImplementRootRTTI(Texture);

    Texture::Texture(const String & texName, const String & sourceName)
        : Resource(texName, sourceName)
        , mHeight(0)
        , mWidth(0)
		, mMipmaps(0)
		, mMipmapLevel(0)
        , mUsage(eUsage::STATIC)
        , mFormat(ePixelFormat::UNKNOWN)
    {
		i_inv_width = 0;
		i_inv_height = 0;
		i_ascept = 0;
    }

    Texture::~Texture()
    {
    }

	void Texture::Release()
	{
		HWBufferManager::Instance()->_delTexture(this);
	}

	void Texture::SampleColor(Float4 & color, float fu, float fv)
	{
		fu *= mWidth - 1;
		fv *= mHeight - 1;

		int iu0 = (int)fu, iv0 = (int)fv;
		int iu1 = iu0 + 1, iv1 = iv0 + 1;

		float du = fu - iu0;
		float dv = fv - iv0;

		iu0 = Math::Clamp(iu0, 0, mWidth - 1);
		iu1 = Math::Clamp(iu1, 0, mWidth - 1);
		iv0 = Math::Clamp(iv0, 0, mHeight - 1);
		iv1 = Math::Clamp(iv1, 0, mHeight - 1);

		Float4 c00, c01, c10, c11;
		GetColorData(c00, iu0, iv0);
		GetColorData(c01, iu1, iv0);
		GetColorData(c10, iu0, iv1);
		GetColorData(c11, iu1, iv1);

		c00 = Float4::Lerp(c00, c01, du);
		c10 = Float4::Lerp(c10, c11, du);

		color = Float4::Lerp(c00, c10, dv);
	}

	void Texture::StrechBlt(Texture * dest, const RectI * _rcDest, const RectI * _rcSrc)
	{
		RectI rcDest, rcSrc;
		if (_rcDest)
		{
			rcDest = *_rcDest;
		}
		else
		{
			rcDest.x1 = 0;
			rcDest.y1 = 0;
			rcDest.x2 = dest->GetWidth() - 1;
			rcDest.y2 = dest->GetHeight() - 1;
		}

		if (_rcSrc)
		{
			rcSrc = *_rcSrc;
		}
		else
		{
			rcSrc.x1 = 0;
			rcSrc.y1 = 0;
			rcSrc.x2 = mWidth - 1;
			rcSrc.y2 = mHeight - 1;
		}

		if (rcSrc.Width() == rcDest.Width() && rcSrc.Height() == rcDest.Height())
		{
			for (int j = 0; j < rcDest.Height(); ++j)
			{
				for (int i = 0; i < rcDest.Width(); ++i)
				{
					Float4 data;
					GetColorData(data, rcSrc.x1 + i, rcSrc.y1 + j);

					dest->SetColorData(data, rcDest.x1 + i, rcDest.y1 + j);
				}
			}
		}
		else
		{
			float u_step = (float)(rcDest.Width() - 1) / (rcSrc.Width() - 1);
			float v_step = (float)(rcDest.Height() - 1) / (rcSrc.Height() - 1);
			float invWidth = 1.0f / (mWidth - 1);
			float invHeight = 1.0f / (mHeight - 1);

			for (int j = 0; j < rcDest.Height(); ++j)
			{
				for (int i = 0; i < rcDest.Width(); ++i)
				{
					float u = rcSrc.x1 + i * u_step;
					float v = rcSrc.y1 + j * v_step;

					u *= invWidth;
					v *= invHeight;

					Float4 data;
					SampleColor(data, u, v);
					
					dest->SetColorData(data, rcDest.x1 + i, rcDest.y1 + j);
				}
			}
		}
	}

	void Texture::_update()
	{
		i_inv_width = 1.0f / mWidth;
		i_inv_height = 1.0f / mHeight;

		if (mHeight != 0)
			i_ascept = mWidth / (float)mHeight;
	}
}