#include "MImageBMP.h"
#include "MLog.h"

namespace Rad {

	struct BMP_Header
	{
		word type;
		dword sizefile;
		word reserved1;
		word reserved2;
		dword offbytes;

		union BMP_HeaderInfo
		{
			struct {
				int size;
				int width;
				int height;
				short plane;
				short bitcount;
				int compression;
				int sizeimage;
				int xpelspermeter;
				int ypelspermeter;
				int clrused;
				int clrimportant;
			};
			
			byte m[40];

		} info;
	};

	struct BMP_RgbQuad
	{
		byte b;
		byte g;
		byte r;
		byte a;
	};

	bool BMP_Test(DataStreamPtr stream)
	{
		d_assert (stream != NULL);
		
		word type = 0;
		
		int nreads = stream->Read(&type, sizeof(type));

		stream->Skip(-nreads);

		return type == 0x4d42;
	}

	bool BMP_Load(Image & image, DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		DataStream & IS = *stream.c_ptr();

		BMP_Header header;
		header.type = 0;

		IS >> header.type;
		IS >> header.sizefile;
		IS >> header.reserved1;
		IS >> header.reserved2;
		IS >> header.offbytes;

		if (header.type != 0x4d42)
			return false;

		header.info.size = 0;
		IS.Read(&header.info, 40);
	
		if (header.info.size != 40)
		{
			d_log("?: BMP format not supported");
			return false;
		}

		if (header.info.compression == 1 || header.info.compression == 2)
		{
			d_log("?: BMP compression 'RLE', not supported");
			return false;
		}

		bool flip_vertically = header.info.height > 0;
		header.info.height = abs(header.info.height);

		if (header.info.width > MAX_HW_TEXTURE_SIZE || header.info.height > MAX_HW_TEXTURE_SIZE)
		{
			d_log("?: BMP size too large");
			return false;
		}

		image.width = header.info.width;
		image.height = header.info.height;
		image.depth = 1;
		image.mipmaps = 0;
		image.cubmaps = 1;

		if (header.info.bitcount == 8)
		{
			BMP_RgbQuad palette[256];
			IS.Read(palette, sizeof(BMP_RgbQuad) * 256);

			byte * data = new byte[image.width * image.height];
			IS.Read(data, image.width * image.height);

			image.pixels = new byte[image.width * image.height * 3];
			for (int i = 0; i < image.width * image.height; ++i)
			{
				image.pixels[i * 3 + 0] = palette[data[i]].r;
				image.pixels[i * 3 + 1] = palette[data[i]].g;
				image.pixels[i * 3 + 2] = palette[data[i]].b;
			}

			delete[] data;

			image.format = ePixelFormat::R8G8B8;
		}
		else if (header.info.bitcount == 16)
		{
			// here not ensure right
			image.pixels = new byte[image.width * image.height * 2];
			IS.Read(image.pixels, image.width * image.height * 2);

			image.format = ePixelFormat::R5G6B5;
		}
		else if (header.info.bitcount == 24)
		{
			image.pixels = new byte[image.width * image.height * 3];

			IS.Read(image.pixels, image.width * image.height * 3);
			for (int i = 0; i < image.width * image.height; ++i)
			{
				Swap(image.pixels[i * 3 + 0], image.pixels[i * 3 + 2]);
			}
			
			image.format = ePixelFormat::R8G8B8;
		}
		else if (header.info.bitcount == 32)
		{
			image.pixels = new byte[image.width * image.height * 4];
			IS.Read(image.pixels, image.width * image.height * 4);
			for (int i = 0; i < image.width * image.height; ++i)
			{
				Swap(image.pixels[i * 4 + 0], image.pixels[i * 4 + 2]);
			}

			image.format = ePixelFormat::R8G8B8A8;
		}
		else
		{
			d_log("?: BMP format not supported");
			return false;
		}

		if (flip_vertically)
		{
			int line_bytes = image.width * header.info.bitcount / 8;
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

	bool BMP_Save(OSerializer & OS, const byte * pixels, int width, int height, ePixelFormat format)
	{
		if (format != ePixelFormat::R8G8B8 && format != ePixelFormat::R8G8B8A8)
		{
			d_log("?: BMP format not supported");
			return false;
		}

		int bitcount = format == ePixelFormat::R8G8B8 ? 24 : 32;
		int chanels = bitcount / 8;
		int line_bytes = width * chanels;

		BMP_Header header;
		header.type = 0x4d42;
		header.sizefile = 14 + 40 + line_bytes * height;
		header.reserved1 = 0;
		header.reserved2 = 0;
		header.offbytes = 14 + 40;

		OS << header.type;
		OS << header.sizefile;
		OS << header.reserved1;
		OS << header.reserved2;
		OS << header.offbytes;

		header.info.size = 40;
		header.info.width = width;
		header.info.height = height;
		header.info.plane = 1;
		header.info.bitcount = bitcount;
		header.info.compression = 0;
		header.info.sizeimage = line_bytes * height;
		header.info.xpelspermeter = 0;
		header.info.ypelspermeter = 0;
		header.info.clrused = 0;
		header.info.clrimportant = 0;

		OS.Write(&header.info, 40);

		for (int j = height - 1; j >= 0; --j)
		{
			const byte * c_pixels = pixels + line_bytes * j;

			if (bitcount == 24 || bitcount == 32)
			{
				static_memory buffer;

				byte * data = (byte *)buffer.query(M_STATIC_MEMORY_I_CHANEL, line_bytes);
				memcpy(data, c_pixels, line_bytes);

				for (int i = 0; i < width; ++i)
				{
					Swap(data[i * chanels + 0], data[i * chanels + 2]);
				}

				OS.Write(data, line_bytes);
			}
			else
			{
				// ??? 16 bit
				OS.Write(c_pixels, line_bytes);
			}
		}

		return true;
	}

}