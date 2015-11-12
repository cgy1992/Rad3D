#include "NullTexture.h"
#include "MLog.h"
#include "MImage.h"

namespace Rad {

	ImplementRTTI(NullTexture, Texture);

	NullTexture::NullTexture(const String & texName, const String & sourceName)
		: Texture(texName, sourceName)
		, mPixelData(NULL)
		, mLockFlag(0)
	{
	}

	NullTexture::~NullTexture()
	{
		OnUnload();

		safe_delete_array(mPixelData);
	}

	void NullTexture::OnLoad()
	{
		Resource::OnLoad();

		_update();
	}

	void NullTexture::OnUnload()
	{
		Resource::OnUnload();
	}

	void NullTexture::_loadImp(DataStreamPtr stream)
	{
		if (mLoadState != Resource::LOADING)
			return ;

		if (stream == NULL)
		{
			d_log("!: Load texture '%s' failed.", mSourceName.c_str());
			return ;
		}

		d_assert (mPixelData == NULL && mLockFlag == 0);

		Image image;
		if (IMG_Load(image, stream))
		{
			mPixelData = image.pixels;
			mWidth = image.width;
			mHeight = image.height;
			mFormat = image.format;
			mMipmaps = 1;
		}
		else
		{
			d_log ("?: image '%s' load failed.", mSourceName.c_str());
		}
	}

	static int GetPixelFormatBytes(ePixelFormat Format)
	{
		switch (Format)
		{
		case ePixelFormat::R8G8B8A8:
			return 4;

		case ePixelFormat::R8G8B8:
			return 3;

		case ePixelFormat::L8A8:
			return 2;

		case ePixelFormat::L8:
			return 1;

		case ePixelFormat::R5G6B5:
		case ePixelFormat::R5G5B5A1:
		case ePixelFormat::R4G4B4A4:
			return 2;

		case ePixelFormat::R16F:
			return 2;
		case ePixelFormat::R16G16F:
			return 4;
		case ePixelFormat::R16G16B16A16F:
			return 8;

		case ePixelFormat::R32F:
			return 4;
		case ePixelFormat::R32G32F:
			return 8;
		case ePixelFormat::R32G32B32A32F:
			return 16;
		}

		d_assert (0);

		return 0;
	}

	void * NullTexture::Lock(eLockFlag flag)
	{
		d_assert (mLockFlag == 0);

		mLockFlag = flag;

		if (mPixelData == NULL)
		{
			int btPixel = GetPixelFormatBytes(mFormat);
			d_assert (btPixel != 0);

			mPixelData = new byte[btPixel * mWidth * mHeight];
			memset(mPixelData, 0, btPixel * mWidth * mHeight);
		}

		return mPixelData;
	}

	void NullTexture::Unlock()
	{
		d_assert (mLockFlag != 0);

		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
		{
			safe_delete_array(mPixelData);
		}

		mLockFlag = 0;
	}

	bool NullTexture::SetColorData(const Float4 & color, int u, int v)
	{
		d_assert (
			u >= 0 && u < mWidth &&
			v >= 0 && v < mHeight);

		bool hr = true;

		Lock(eLockFlag::WRITE);

		unsigned char r = (unsigned char)(color.r * 255);
		unsigned char g = (unsigned char)(color.g * 255);
		unsigned char b = (unsigned char)(color.b * 255);
		unsigned char a = (unsigned char)(color.a * 255);

		unsigned char * data = (unsigned char *)mPixelData;
		int index = v * mWidth + u;
		switch (mFormat)
		{
		case ePixelFormat::L8:
			{
				data[index] = r;
			}
			break;

		case ePixelFormat::L8A8:
			{
				data[index * 2 + 0] = r;
				data[index * 2 + 1] = a;
			}
			break;

		case ePixelFormat::R8G8B8:
			{
				data[index * 3 + 0] = r;
				data[index * 3 + 1] = g;
				data[index * 3 + 2] = b;
			}
			break;

		case ePixelFormat::R8G8B8A8:
			{
				data[index * 4 + 0] = r;
				data[index * 4 + 1] = g;
				data[index * 4 + 2] = b;
				data[index * 4 + 3] = a;
			}
			break;

		default:
			hr = false;
			break;
		}

		Unlock();

		return true;
	}

	bool NullTexture::GetColorData(Float4 & color, int u, int v)
	{
		d_assert (
			u >= 0 && u < mWidth &&
			v >= 0 && v < mHeight);

		bool hr = true;

		Lock(eLockFlag::READ);

		unsigned char * data = (unsigned char *)mPixelData;
		int index = v * mWidth + u;
		switch (mFormat)
		{
		case ePixelFormat::L8:
			{
				unsigned char r = data[index];

				color.r = r / 255.0f;
				color.g = color.r;
				color.b = color.r;
				color.a = 1;
			}
			break;

		case ePixelFormat::L8A8:
			{
				unsigned char r = data[index * 2];
				unsigned char a = data[index * 2 + 1];

				color.r = r / 255.0f;
				color.g = color.r;
				color.b = color.r;
				color.a = a / 255.0f;
			}
			break;

		case ePixelFormat::R8G8B8:
			{
				unsigned char r = data[index * 3];
				unsigned char g = data[index * 3 + 1];
				unsigned char b = data[index * 3 + 2];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = 1;
			}
			break;

		case ePixelFormat::R8G8B8A8:
			{
				unsigned char r = data[index * 4];
				unsigned char g = data[index * 4 + 1];
				unsigned char b = data[index * 4 + 2];
				unsigned char a = data[index * 4 + 3];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = a / 255.0f;
			}
			break;

		case ePixelFormat::DXT1_RGB:
		case ePixelFormat::DXT3_RGBA:
		case ePixelFormat::DXT5_RGBA:
			{
				int blockSize = mFormat == ePixelFormat::DXT1_RGB ? 8 : 16;
				int blockx = (u + 3) / 4, blocky = (v + 3) / 4;
				int offset = blocky * ((mWidth + 3) / 4) + blockx;
				byte pixel64[64];

				DDS_DecodeBlock(pixel64, data + offset * blockSize, mFormat);

				int j = u - blockx * 4, i = u - blockx * 4;

				index = (j * 16) + i * 4;

				unsigned char r = data[index + 0];
				unsigned char g = data[index + 1];
				unsigned char b = data[index + 2];
				unsigned char a = data[index + 3];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = a / 255.0f;
			}
			break;

		case ePixelFormat::ETC1_RGB:
			{
				int blockSize = 8;
				int blockx = (u + 3) / 4, blocky = (v + 3) / 4;
				int offset = blocky * ((mWidth + 3) / 4) + blockx;
				byte pixel64[64];

				ETC_DecodeBlock(pixel64, data + offset * blockSize, mFormat);

				int j = u - blockx * 4, i = u - blockx * 4;

				index = (j * 16) + i * 4;

				unsigned char r = data[index + 0];
				unsigned char g = data[index + 1];
				unsigned char b = data[index + 2];
				unsigned char a = data[index + 3];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = a / 255.0f;
			}
			break;

		default:
			hr = false;
			break;
		}

		Unlock();

		return hr;
	}

}