#include "MImageETC.h"
#include "etc1.cpp"

namespace Rad {

	bool ETC_Test(DataStreamPtr stream)
	{
		char header[ETC_PKM_HEADER_SIZE];

		int nreads = stream->Read(header, ETC_PKM_HEADER_SIZE);

		stream->Skip(-nreads);

		return nreads == ETC_PKM_HEADER_SIZE && etc1_pkm_is_valid((const etc1_byte * )header) != 0;
	}

	bool ETC_Load(Image & image, DataStreamPtr stream)
	{
		etc1_byte header[ETC_PKM_HEADER_SIZE];
		if (stream->Read(header, ETC_PKM_HEADER_SIZE) != ETC_PKM_HEADER_SIZE || !etc1_pkm_is_valid(header))
			return false;

		int mapSize = stream->Size() - ETC_PKM_HEADER_SIZE;

		image.width = etc1_pkm_get_width(header);
		image.height = etc1_pkm_get_height(header);
		image.depth = 1;
		image.chanels = 3;
		image.bitcount = 0;
		image.cubmaps = 1;
		image.mipmaps = 0;
		image.format = ePixelFormat::ETC1_RGB;

		if (image.width > MAX_HW_TEXTURE_SIZE || image.height > MAX_HW_TEXTURE_SIZE)
			return false;
	
		image.pixels = new byte[mapSize];

		stream->Read(image.pixels, mapSize);

		return true;
	}

	byte * ETC_Decode(const byte * blocks, int width, int height, ePixelFormat format)
	{
		format = ePixelFormat::R8G8B8;

		int linebytes = width * 3;
		byte * pixels = new byte[linebytes * height * 3];

		if (etc1_decode_image(pixels, pixels, width, height, 3, linebytes) == 0)
		{
			return pixels;
		}

		delete[] pixels;

		return NULL;
	}

	void ETC_DecodeBlock(byte pixels[16*4], const byte * iblock, ePixelFormat format)
	{
		d_assert (format == ePixelFormat::ETC1_RGB);

		byte c3[16 * 3];

		etc1_decode_block(iblock, c3);

		for (int i = 0; i < 16; ++i)
		{
			pixels[i * 4 + 0] = c3[i * 3 + 0];
			pixels[i * 4 + 1] = c3[i * 3 + 1];
			pixels[i * 4 + 2] = c3[i * 3 + 2];
			pixels[i * 4 + 3] = 255;
		}
	}

}