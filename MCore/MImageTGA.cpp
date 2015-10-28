#include "MImageTGA.h"
#include "MLog.h"

namespace Rad {

	bool TGA_Test(DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		DataStream & IS = *stream.c_ptr();

		byte TGAHeader[12];
		byte TGACompare[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		int nreads = IS.Read(TGAHeader, 12);

		stream->Skip(-nreads);

		return memcmp(TGAHeader, TGACompare, 12) == 0;
	}

	bool TGA_Load(Image & image, DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		DataStream & IS = *stream.c_ptr();

		byte TGAHeader[12];
		byte TGACompare[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		IS.Read(TGAHeader, 12);

		if (memcmp(TGAHeader, TGACompare, 12) != 0)
		{
			d_log("?: not TGA file");
			return false;
		}

		byte header[6];
		IS.Read(header, 6);

		if (header[4] != 24 && header[4] != 32)
		{
			d_log("?: TGA chanels error");
			return false;
		}

		image.width = header[1] * 256 + header[0];
		image.height = header[3] * 256 + header[2];
		image.depth = 1;
		image.mipmaps = 0;
		image.cubmaps = 1;
		image.bitcount = header[4];
		image.chanels = image.bitcount == 24 ? 3 : 4;
		image.format = image.bitcount == 24 ? ePixelFormat::R8G8B8 : ePixelFormat::R8G8B8A8;

		if (image.width > MAX_HW_TEXTURE_SIZE || image.height > MAX_HW_TEXTURE_SIZE)
		{
			d_log("?: TGA size too large");
			return false;
		}

		image.pixels = new byte[image.width * image.height * image.chanels];

		IS.Read(image.pixels, image.width * image.height * image.chanels);

		int count = image.width * image.height * image.chanels;
		for (int i = 0; i < count; i += image.chanels)
		{
			Swap(image.pixels[i], image.pixels[i + 2]);
		}

		if (image.pixels != NULL)
		{
			int line_bytes = image.width * image.bitcount / 8;
			static_memory buffer;

			buffer.query(M_STATIC_MEMORY_I_CHANEL, line_bytes);
			for (int i = 0, j = image.height - 1; i < j; ++i, --j)
			{
				memcpy (buffer.data(), &image.pixels[i * line_bytes], line_bytes);
				memcpy (&image.pixels[i * line_bytes], &image.pixels[j * line_bytes], line_bytes);
				memcpy (&image.pixels[j * line_bytes], buffer.data(), line_bytes);
			}
		}

		return true;
	}

	bool TGA_Save(OSerializer & OS, const byte * pixels, int width, int height, ePixelFormat format)
	{
		if (format != ePixelFormat::R8G8B8 && format != ePixelFormat::R8G8B8A8)
		{
			d_log("?: TGA format not supported");
			return false;
		}

		int bitcount = format == ePixelFormat::R8G8B8 ? 24 : 32;
		int chanels = bitcount / 8;
		int line_bytes = width * chanels;

		byte TGAHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		OS.Write(TGAHeader, 12);

		byte header[6];
		header[0] = width & 255;
		header[1] = width / 256;
		header[2] = height & 255;
		header[3] = height / 256;
		header[4] = format == ePixelFormat::R8G8B8 ? 24 : 32;
		header[5] = 0;

		OS.Write(header, 6);

		for (int j = height - 1; j >= 0; --j)
		{
			const byte * c_pixels = pixels + line_bytes * j;
			static_memory buffer;

			byte * data = (byte *)buffer.query(M_STATIC_MEMORY_I_CHANEL, line_bytes);
			memcpy(data, c_pixels, line_bytes);

			for (int i = 0; i < width; ++i)
			{
				Swap(data[i * chanels + 0], data[i * chanels + 2]);
			}

			OS.Write(data, line_bytes);
		}

		return true;
	}

}

