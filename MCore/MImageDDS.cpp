#include "MImageDDS.h"

namespace Rad {

	struct DDS_Format 
	{
		dword size;
		dword flags;
		dword fourcc;
		dword bitcount;
		dword rmask;
		dword gmask;
		dword bmask;
		dword amask;
	};

	struct DDS_Caps 
	{
		dword caps1;
		dword caps2;
		dword ddsx;
		dword reserved;
	};
	
	struct DDS_Header
	{
		dword magic;
		dword size;
		dword flags;
		dword height;
		dword width;
		dword pitch;
		dword depth;
		dword mipmap;
		dword reserved[11];

		DDS_Format format;
		DDS_Caps caps;

		dword reserved2;
	};

#define DDSD_CAPS	0x00000001
#define DDSD_HEIGHT	0x00000002
#define DDSD_WIDTH	0x00000004
#define DDSD_PITCH	0x00000008
#define DDSD_PIXELFORMAT	0x00001000
#define DDSD_MIPMAPCOUNT	0x00020000
#define DDSD_LINEARSIZE	0x00080000
#define DDSD_DEPTH	0x00800000

#define DDPF_ALPHAPIXELS	0x00000001
#define DDPF_FOURCC	0x00000004
#define DDPF_RGB	0x00000040

#define DDSCAPS_COMPLEX	0x00000008
#define DDSCAPS_TEXTURE	0x00001000
#define DDSCAPS_MIPMAP	0x00400000

#define DDSCAPS2_CUBEMAP	0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX	0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX	0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY	0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY	0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ	0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ	0x00008000
#define DDSCAPS2_VOLUME	0x00200000

#define DDS_DWORD(c0, c1, c2, c3) MAKE_DWORD(c3, c2, c1, c0)
#define FOURCC_DXT1  (DDS_DWORD('D','X','T','1'))
#define FOURCC_DXT2  (DDS_DWORD('D','X','T','2'))
#define FOURCC_DXT3  (DDS_DWORD('D','X','T','3'))
#define FOURCC_DXT4  (DDS_DWORD('D','X','T','4'))
#define FOURCC_DXT5  (DDS_DWORD('D','X','T','5'))

	bool DDS_Test(DataStreamPtr stream)
	{
		d_assert (stream != NULL);

		int nreads = 0;
		int magic, size = 0;

		nreads += stream->Read(&magic, sizeof(int));
		nreads += stream->Read(&size, sizeof(int));

		stream->Skip(-nreads);

		return magic == DDS_DWORD('D', 'D', 'S', ' ') && size == 124;
	}

	bool DDS_Load(Image & image, DataStreamPtr stream)
	{
		DataStream & IS = *stream.c_ptr();

		d_assert (sizeof(DDS_Header) == 128);

		DDS_Header header;

		IS >> header;

		int flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		if (header.magic != DDS_DWORD('D', 'D', 'S', ' ') || header.size != 124 || (header.flags & flags) != flags)
			return false;

		if (header.width > MAX_HW_TEXTURE_SIZE || header.height > MAX_HW_TEXTURE_SIZE)
			return false;

		flags = DDPF_FOURCC | DDPF_RGB;
		if (header.format.size != 32 || (header.format.flags & flags) == 0)
			return false;

		if ((header.caps.caps1 & DDSCAPS_TEXTURE) == 0)
			return false;

		int is_compressed = (header.format.flags & DDPF_FOURCC) / DDPF_FOURCC;
		int has_alpha = (header.format.flags & DDPF_ALPHAPIXELS) / DDPF_ALPHAPIXELS;

		image.width = header.width;
		image.height = header.height;
		image.depth = header.depth;
		image.mipmaps = header.mipmap;

		if (is_compressed)
		{
			int blockSize = 0;
			switch (header.format.fourcc)
			{
			case FOURCC_DXT1:
				image.format = ePixelFormat::DXT1_RGB;
				blockSize = 8;
				break;

			case FOURCC_DXT2:
			case FOURCC_DXT3:
				image.format = ePixelFormat::DXT3_RGBA;
				blockSize = 16;
				break;

			case FOURCC_DXT4:
			case FOURCC_DXT5:
				image.format = ePixelFormat::DXT5_RGBA;
				blockSize = 16;
				break;
			}

			if (blockSize == 0)
				return false;

			if ((header.caps.caps2 & DDSCAPS2_CUBEMAP) && image.width == image.height)
				image.cubmaps = 6;
			else
				image.cubmaps = 1;

			int mapSize = 0;
			int w = image.width, h = image.height;
			for (int i = 0; i <= image.mipmaps; ++i)
			{
				mapSize += ((w + 3) / 4) * ((h + 3) / 4);

				if ((w /= 2) == 0) w = 1;
				if ((h /= 2) == 0) w = 1;
			}

			image.pixels = new byte[mapSize * blockSize];

			IS.Read(image.pixels, mapSize * blockSize);
		}
		else
		{
			int chanels = has_alpha ? 4 : 3;
			int sizeOfBytes = image.width * image.height * chanels;

			image.depth = 1;
			image.mipmaps = 0;
			image.format = !has_alpha ? ePixelFormat::R8G8B8 : ePixelFormat::R8G8B8A8;

			image.pixels = new byte[sizeOfBytes];

			IS.Read(image.pixels, sizeOfBytes);

			for (int i = 0; i < sizeOfBytes; i += chanels)
			{
				Swap(image.pixels[i + 0], image.pixels[i + 2]);
			}
		}

		return true;
	}

	template <int bitcount>
	inline byte DDS_BitLimit(byte i)
	{
		return i & ((1 << bitcount) - 1);
	}

	void DXT1_Uncompress(
		byte uncompressed[16*4],
		const byte compressed[8])
	{
		int c0, c1;
		Field<byte, 4*4> colors;

		c0 = compressed[0] + (compressed[1] << 8);
		c1 = compressed[2] + (compressed[3] << 8);

		colors[0] = DDS_BitLimit<5>(c0 >> 11) * (256 / 32);
		colors[1] = DDS_BitLimit<6>(c0 >> 05) * (256 / 64);
		colors[2] = DDS_BitLimit<5>(c0 >> 00) * (256 / 32);
		colors[3] = 255;

		colors[4] = DDS_BitLimit<5>(c1 >> 11) * (256 / 32);
		colors[5] = DDS_BitLimit<6>(c1 >> 05) * (256 / 64);
		colors[6] = DDS_BitLimit<5>(c1 >> 00) * (256 / 32);
		colors[7] = 255;

		if(c0 > c1)
		{
			// no alpha
			colors[8] = (2*colors[0] + colors[4]) / 3;
			colors[9] = (2*colors[1] + colors[5]) / 3;
			colors[10] = (2*colors[2] + colors[6]) / 3;
			colors[11] = 255;
			colors[12] = (colors[0] + 2*colors[4]) / 3;
			colors[13] = (colors[1] + 2*colors[5]) / 3;
			colors[14] = (colors[2] + 2*colors[6]) / 3;
			colors[15] = 255;
		}
		else
		{
			colors[8] = (colors[0] + colors[4]) / 2;
			colors[9] = (colors[1] + colors[5]) / 2;
			colors[10] = (colors[2] + colors[6]) / 2;
			colors[11] = 255;
			colors[12] = 0;
			colors[13] = 0;
			colors[14] = 0;
			colors[15] = 0;
		}

		int next_bit = 4*8;
		for(int i = 0; i < 16*4; i += 4)
		{
			int idx = ((compressed[next_bit>>3] >> (next_bit & 7)) & 3) * 4;
			uncompressed[i+0] = colors[idx+0];
			uncompressed[i+1] = colors[idx+1];
			uncompressed[i+2] = colors[idx+2];
			uncompressed[i+3] = colors[idx+3];

			next_bit += 2;
		}
	}

	void DXT3_UncompressAlpha(
		byte uncompressed[16*4],
		const byte compressed[8])
	{
		int next_bit = 0;
		for(int i = 3; i < 16*4; i += 4)
		{
			uncompressed[i] = DDS_BitLimit<4>(compressed[next_bit>>3] >> (next_bit&7));
			uncompressed[i] *= (255 / 16);
			next_bit += 4;
		}
	}

	void DXT5_UncompressAlpha(
		byte uncompressed[16*4],
		const byte compressed[8])
	{
		byte alpha[8];

		alpha[0] = compressed[0];
		alpha[1] = compressed[1];
		if( alpha[0] > alpha[1] )
		{
			alpha[2] = (6*alpha[0] + 1*alpha[1]) / 7;
			alpha[3] = (5*alpha[0] + 2*alpha[1]) / 7;
			alpha[4] = (4*alpha[0] + 3*alpha[1]) / 7;
			alpha[5] = (3*alpha[0] + 4*alpha[1]) / 7;
			alpha[6] = (2*alpha[0] + 5*alpha[1]) / 7;
			alpha[7] = (1*alpha[0] + 6*alpha[1]) / 7;
		}
		else
		{
			alpha[2] = (4*alpha[0] + 1*alpha[1]) / 5;
			alpha[3] = (3*alpha[0] + 2*alpha[1]) / 5;
			alpha[4] = (2*alpha[0] + 3*alpha[1]) / 5;
			alpha[5] = (1*alpha[0] + 4*alpha[1]) / 5;
			alpha[6] = 0;
			alpha[7] = 255;
		}

		int next_bit = 8*2;
		for(int i = 3; i < 16*4; i += 4)
		{
			int idx = 0, bit;
			bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
			idx += bit << 0;
			++next_bit;
			bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
			idx += bit << 1;
			++next_bit;
			bit = (compressed[next_bit>>3] >> (next_bit&7)) & 1;
			idx += bit << 2;
			++next_bit;
			uncompressed[i] = alpha[idx & 7];
		}
	}

	void DXT_Uncompress(
		byte uncompressed[16*4],
		const byte compressed[8])
	{
		int c0, c1;
		byte colors[4*3];

		c0 = compressed[0] + (compressed[1] << 8);
		c1 = compressed[2] + (compressed[3] << 8);

		colors[0] = DDS_BitLimit<5>(c0 >> 11) * (256 / 32);
		colors[1] = DDS_BitLimit<6>(c0 >> 05) * (256 / 64);
		colors[2] = DDS_BitLimit<5>(c0 >> 00) * (256 / 32);

		colors[3] = DDS_BitLimit<5>(c1 >> 11) * (256 / 32);
		colors[4] = DDS_BitLimit<6>(c1 >> 05) * (256 / 64);
		colors[5] = DDS_BitLimit<5>(c1 >> 00) * (256 / 32);

		colors[6] = (2*colors[0] + colors[3]) / 3;
		colors[7] = (2*colors[1] + colors[4]) / 3;
		colors[8] = (2*colors[2] + colors[5]) / 3;
		colors[9] = (colors[0] + 2*colors[3]) / 3;
		colors[10] = (colors[1] + 2*colors[4]) / 3;
		colors[11] = (colors[2] + 2*colors[5]) / 3;

		int next_bit = 4*8;
		for(int i = 0; i < 16*4; i += 4)
		{
			int idx = ((compressed[next_bit>>3] >> (next_bit & 7)) & 3) * 3;
			uncompressed[i+0] = colors[idx+0];
			uncompressed[i+1] = colors[idx+1];
			uncompressed[i+2] = colors[idx+2];

			next_bit += 2;
		}
	}

	byte * DDS_Decode(const byte * blocks, int width, int height, ePixelFormat format)
	{
		int blockSize = format == ePixelFormat::DXT1_RGB ? 8 : 16;
		int chanels = format == ePixelFormat::DXT1_RGB ? 3 : 4;

		byte * pixels = new byte[chanels * width * height];
		int x_blocks = (width + 3) / 4;
		int y_blocks = (height + 3) / 4;
		byte color[16 * 4];

		int index = 0;
		for (int j = 0; j < y_blocks; ++j)
		{
			for (int i = 0; i < x_blocks; ++i)
			{
				int bw = 4, bh = 4;
				int rx = 4 * i, ry = 4 * j;

				const byte * compress = blocks + index * blockSize;

				if (format == ePixelFormat::DXT1_RGB)
				{
					DXT1_Uncompress(color, compress);
				}
				else if (format == ePixelFormat::DXT3_RGBA)
				{
					DXT3_UncompressAlpha(color, compress);
					DXT_Uncompress(color, compress + 8);
				}
				else if (format == ePixelFormat::DXT5_RGBA)
				{
					DXT5_UncompressAlpha(color, compress);
					DXT_Uncompress(color, compress + 8);
				}

				if (rx + 4 > width)
				{
					bw = width - rx;
				}

				if (ry + 4 > height)
				{
					bh = height - ry;
				}

				for (int y = 0; y < bh; ++y)
				{
					byte * data = pixels + ((ry + y) * width + rx) * chanels;
					for (int x = 0; x < bw; ++x)
					{
						int ci = x * chanels;;
						int cj = (y * 4 + x) * 4;

						data[ci + 0] = color[cj + 0];
						data[ci + 1] = color[cj + 1];
						data[ci + 2] = color[cj + 2];

						if (chanels == 4)
							data[ci + 3] = color[cj + 3];
					}
				}

				++index;
			}
		}

		return pixels;
	}

	void DDS_DecodeBlock(byte pixel[16*4], const byte iblock[8], ePixelFormat format)
	{
		if (format == ePixelFormat::DXT1_RGB)
		{
			DXT1_Uncompress(pixel, iblock);
		}
		else if (format == ePixelFormat::DXT3_RGBA)
		{
			DXT3_UncompressAlpha(pixel, iblock);
			DXT_Uncompress(pixel, iblock + 8);
		}
		else if (format == ePixelFormat::DXT5_RGBA)
		{
			DXT5_UncompressAlpha(pixel, iblock);
			DXT_Uncompress(pixel, iblock + 8);
		}
		else
		{
			d_assert (0);
		}
	}

}