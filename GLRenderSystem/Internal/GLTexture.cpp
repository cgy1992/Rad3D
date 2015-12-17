#include "GLTexture.h"
#include "GLHWBufferManager.h"
#include "GLUtil.h"
#include "MImage.h"
#include "MRenderSystem.h"

namespace Rad {

	ImplementRTTI(GLTexture, Texture);

	GLTexture::GLTexture(const String & texName, const String & sourceName)
		: Texture(texName, sourceName)
		, mGLTexture(0)
		, mPixelData(NULL)
		, mLockFlag(0)
	{
	}

	GLTexture::~GLTexture()
	{
		OnUnload();

		safe_delete_array(mPixelData);
	}

	void GLTexture::OnLoad()
	{
		Resource::OnLoad();

		_update();
	}

	void GLTexture::OnUnload()
	{
		Resource::OnUnload();

		if (mGLTexture != 0)
		{
			glDeleteTextures(1, &mGLTexture);
			mGLTexture = 0;
		}
	}

	void GLTexture::_load(DataStreamPtr stream)
	{
		if (mLoadState != Resource::LOADING || stream == NULL)
			return ;

		d_assert (mPixelData == NULL && mLockFlag == 0);

		Image image;
		if (IMG_Load(image, stream))
		{
			mPixelData = image.pixels;
			mWidth = image.width;
			mHeight = image.height;
			mFormat = image.format;
			mMipmaps = image.mipmaps != 0 ? image.mipmaps : mMipmaps;

			_checkFormat();

			_build();
		}
		else
		{
			d_log ("?: image '%s' load failed.", mSourceName.c_str());
		}
	}

	void GLTexture::_checkFormat()
	{
		if (RenderSystem::Instance()->GetCaps().pixelFormats[mFormat])
			return ;

		if (mFormat == ePixelFormat::DXT1_RGB ||
			mFormat == ePixelFormat::DXT3_RGBA ||
			mFormat == ePixelFormat::DXT5_RGBA)
		{
			byte * pixels = DDS_Decode(mPixelData, mWidth, mHeight, mFormat);
			delete[] mPixelData;
			mPixelData = pixels;

			mFormat = mFormat == ePixelFormat::DXT1_RGB ? ePixelFormat::R8G8B8 : ePixelFormat::R8G8B8A8;
		}
		else if (mFormat == ePixelFormat::ETC1_RGB)
		{
			byte * pixels = ETC_Decode(mPixelData, mWidth, mHeight, mFormat);
			delete[] mPixelData;
			mPixelData = pixels;

			mFormat = ePixelFormat::R8G8B8;
		}
		else if (mFormat == ePixelFormat::PVRTC_RGB || mFormat == ePixelFormat::PVRTC_RGBA)
		{
			byte * pixels = PVR_Decode(mPixelData, mWidth, mHeight, mFormat);
			delete mPixelData;

			mPixelData = pixels;
			mFormat = ePixelFormat::R8G8B8A8;
		}

		if (mMipmaps > 0)
			mMipmaps = -1;
	}

	void * GLTexture::Lock(eLockFlag flag)
	{
		d_assert (mLockFlag == 0);

		mLockFlag = flag;

#ifdef M_DEBUG
		if (mLockFlag & eLockFlag::READ)
		{
			d_assert (mUsage == eUsage::STATIC_MANAGED || mUsage == eUsage::DYNAMIC_MANAGED);
		}
#endif

		if (mPixelData == NULL)
		{
			int btPixel = GLUtil::GetPixelFormatBytes(mFormat);
			d_assert (btPixel != 0);

			mPixelData = new byte[btPixel * mWidth * mHeight];
			memset(mPixelData, 0, btPixel * mWidth * mHeight);
		}

		return mPixelData;
	}

	void GLTexture::Unlock()
	{
		d_assert (mLockFlag != 0);

		if (mLockFlag & eLockFlag::WRITE)
		{
			_build();
		}
		
		if (mUsage == eUsage::STATIC || mUsage == eUsage::DYNAMIC)
		{
			safe_delete_array(mPixelData);
		}

		mLockFlag = 0;
	}

	bool GLTexture::SetColorData(const Float4 & color, int u, int v)
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

		return hr;
	}

	bool GLTexture::GetColorData(Float4 & color, int u, int v)
	{
		d_assert (
			u >= 0 && u < mWidth &&
			v >= 0 && v < mHeight);

		bool hr = true;
		unsigned char * data = (unsigned char *)mPixelData;

		Lock(eLockFlag::READ);

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

				unsigned char r = pixel64[index + 0];
				unsigned char g = pixel64[index + 1];
				unsigned char b = pixel64[index + 2];
				unsigned char a = pixel64[index + 3];

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

				unsigned char r = pixel64[index + 0];
				unsigned char g = pixel64[index + 1];
				unsigned char b = pixel64[index + 2];
				unsigned char a = pixel64[index + 3];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = a / 255.0f;
			}
			break;

		case ePixelFormat::PVRTC_RGB:
		case ePixelFormat::PVRTC_RGBA:
			{
				int blockSize = 16;
				int blockx = (u + 3) / 4, blocky = (v + 3) / 4;
				int offset = blocky * ((mWidth + 3) / 4) + blockx;
				byte pixel64[64];

				PVR_DecodeBlock(pixel64, data + offset * blockSize, mFormat);

				int j = u - blockx * 4, i = u - blockx * 4;

				index = (j * 16) + i * 4;

				unsigned char r = pixel64[index + 0];
				unsigned char g = pixel64[index + 1];
				unsigned char b = pixel64[index + 2];
				unsigned char a = pixel64[index + 3];

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

	void GLTexture::OnLostDevice()
	{
		if (mGLTexture != 0)
		{
			glDeleteTextures(1, &mGLTexture);
			mGLTexture = 0;
		}
	}

	void GLTexture::OnResetDevice()
	{
		d_assert (mGLTexture == 0);

		_build();
	}

	void GLTexture::_build()
	{
		if (mUsage != eUsage::SYSTEM && mPixelData != NULL)
		{
			if (mGLTexture == 0)
				glGenTextures(1, &mGLTexture);

			d_assert (mWidth >= 4 && "some gpu sample error, when texture width < 4");

			if (mFormat == ePixelFormat::DXT1_RGB ||
				mFormat == ePixelFormat::DXT3_RGBA ||
				mFormat == ePixelFormat::DXT5_RGBA ||
				mFormat == ePixelFormat::ETC1_RGB ||
				mFormat == ePixelFormat::PVRTC_RGB ||
				mFormat == ePixelFormat::PVRTC_RGBA)
			{
				_buildCompressed();
			}
			else
			{
				_buildNormal();
			}

			if (mMipmaps < 0)
			{
				mMipmapLevel = Min(Math::Log2(mWidth), Math::Log2(mHeight));
				if (mMipmapLevel > 0)
				{
#ifdef M_PLATFORM_WIN32
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mMipmapLevel);

					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
			else
			{
				mMipmapLevel = mMipmaps;
			}

			d_assert (glGetError() == 0);
		}
	}

#define SAMPLE_COLOR(i, j, chanels, c) mPixelData[((j) * mWidth + (i)) * (chanels) + (c)]

	void GLTexture::_buildNormal()
	{
		byte * data = mPixelData;
		ePixelFormat format = mFormat;
		int width = mWidth, height = mHeight;
		static_memory memory;

		if ((HWBufferManager::Instance()->IsTextureLowFormat() || HWBufferManager::Instance()->IsTextureLowResolution()) &&
			!HWBufferManager::Instance()->IsTextureSpecial(mName.c_str()) &&
			(mFormat == ePixelFormat::R8G8B8A8 || mFormat == ePixelFormat::R8G8B8))
		{
			memory.query(M_STATIC_MEMORY_I_CHANEL, mWidth * mHeight * 2);

			if (HWBufferManager::Instance()->IsTextureLowResolution() && width > 64 && height > 64)
			{
				byte * pixels = (byte *)memory.data();

				width /= 2, height /= 2;
				
				if (mFormat == ePixelFormat::R8G8B8A8)
				{
					for (int j = 0; j < height; ++j)
					{
						for (int i = 0; i < width; ++i)
						{
							int r = 0, g = 0, b = 0, a = 0;
							
							r += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 4, 0);
							g += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 4, 1);
							b += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 4, 2);
							a += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 4, 3);

							r += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 4, 0);
							g += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 4, 1);
							b += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 4, 2);
							a += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 4, 3);

							r += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 4, 0);
							g += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 4, 1);
							b += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 4, 2);
							a += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 4, 3);

							r += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 4, 0);
							g += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 4, 1);
							b += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 4, 2);
							a += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 4, 3);

							r /= 4, g /= 4, b /= 4, a /= 4;

							pixels[(j * width + i) * 4 + 0] = r;
							pixels[(j * width + i) * 4 + 1] = g;
							pixels[(j * width + i) * 4 + 2] = b;
							pixels[(j * width + i) * 4 + 3] = a;
						}
					}

					data = (byte *)pixels;
				}
				else if (mFormat == ePixelFormat::R8G8B8)
				{
					for (int j = 0; j < height; ++j)
					{
						for (int i = 0; i < width; ++i)
						{
							int r = 0, g = 0, b = 0;

							r += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 3, 0);
							g += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 3, 1);
							b += SAMPLE_COLOR(i * 2 + 0, j * 2 + 0, 3, 2);

							r += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 3, 0);
							g += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 3, 1);
							b += SAMPLE_COLOR(i * 2 + 1, j * 2 + 0, 3, 2);

							r += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 3, 0);
							g += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 3, 1);
							b += SAMPLE_COLOR(i * 2 + 0, j * 2 + 1, 3, 2);

							r += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 3, 0);
							g += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 3, 1);
							b += SAMPLE_COLOR(i * 2 + 1, j * 2 + 1, 3, 2);

							r /= 4, g /= 4, b /= 4;

							pixels[(j * width + i) * 3 + 0] = r;
							pixels[(j * width + i) * 3 + 1] = g;
							pixels[(j * width + i) * 3 + 2] = b;
						}
					}

					data = (byte *)pixels;
				}
			}

			if (HWBufferManager::Instance()->IsTextureLowFormat())
			{
				if (mFormat == ePixelFormat::R8G8B8A8)
				{
					word * pixels = (word *)memory.data();
					for (int i = 0; i < width * height; ++i)
					{
						int r = data[i * 4 + 0];
						int g = data[i * 4 + 1];
						int b = data[i * 4 + 2];
						int a = data[i * 4 + 3];

						r /= 16, g /= 16, b /= 16, a /= 16;

						pixels[i] = (word)(r << 12 | g << 8 | b << 4 | a);
					}

					data = (byte *)pixels;
					format = ePixelFormat::R4G4B4A4;
				}
				else if (mFormat == ePixelFormat::R8G8B8)
				{
					word * pixels = (word *)memory.data();
					for (int i = 0; i < width * height; ++i)
					{
						int r = data[i * 3 + 0];
						int g = data[i * 3 + 1];
						int b = data[i * 3 + 2];

						r /= 8, g /= 4, b /= 8;

						pixels[i] = (word)(r << 11 | g << 5 | b);
					}

					data = (byte *)pixels;
					format = ePixelFormat::R5G6B5;
				}
			}
		}

		GLenum _internal = GLUtil::GetGLInernalFormat(format);
		GLenum _format = GLUtil::GetGLPixelFormat(format);
		GLenum _type = GLUtil::GetGLPixelType(format);

		glBindTexture(GL_TEXTURE_2D, mGLTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, _internal, width, height, 0, _format, _type, data);
	}

	void GLTexture::_buildCompressed()
	{
		GLenum _format = GLUtil::GetGLCrompressedPixelFormat(mFormat);
		if (_format == 0)
			return ;

		glBindTexture(GL_TEXTURE_2D, mGLTexture);

		byte * pixels = mPixelData;
		int width = mWidth;
		int height = mHeight;
		int blockSize = 0;

		switch (mFormat)
		{
		case ePixelFormat::DXT1_RGB:
			blockSize = 8;
			break;

		case ePixelFormat::DXT3_RGBA:
		case ePixelFormat::DXT5_RGBA:
			blockSize = 16;
			break;

		case ePixelFormat::ETC1_RGB:
			blockSize = 8;
			break;

		case ePixelFormat::PVRTC_RGB:
		case ePixelFormat::PVRTC_RGBA:
			blockSize = 16;
			break;
		}

		d_assert (blockSize != 0);

		int maps = mMipmaps >= 0 ? mMipmaps + 1 : 1;
		for (int i = 0; i < maps; ++i)
		{
			int mapSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

			glCompressedTexImage2D(GL_TEXTURE_2D, i, _format, width, height, 0, mapSize, pixels);

			pixels += mapSize;
			if ((width /= 2) == 0) width = 1;
			if ((height /= 2) == 0) height = 1;
		}
	}

}