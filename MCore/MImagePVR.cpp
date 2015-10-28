#include "MImagePVR.h"

namespace Rad {

#define PVR_FLAG_TYPE_MASK 0xFF

	const char PVR_Identifier[5] = "PVR!";

	enum {
		kPVRTextureFlagTypePVRTC_2 = 24,
		kPVRTextureFlagTypePVRTC_4
	};

	struct PVR_Header
	{
		uint32_t headerLength;
		uint32_t height;
		uint32_t width;
		uint32_t numMipmaps;
		uint32_t flags;
		uint32_t dataLength;
		uint32_t bpp;
		uint32_t bitmaskRed;
		uint32_t bitmaskGreen;
		uint32_t bitmaskBlue;
		uint32_t bitmaskAlpha;
		uint32_t pvrTag;
		uint32_t numSurfs;
	};

	bool PVR_Test(DataStreamPtr stream)
	{
		PVR_Header header;

		int nreads = stream->Read(&header, sizeof(PVR_Header));

		stream->Skip(-nreads);

		return nreads > 4 && memcmp(&header.pvrTag, PVR_Identifier, 4) == 0;
	}

	bool PVR_Load(Image & image, DataStreamPtr stream)
	{
		if (!PVR_Test(stream))
			return false;

		PVR_Header header;
		if (stream->Read(&header, sizeof(PVR_Header)) == 0 ||
			memcmp(&header.pvrTag, PVR_Identifier, 4) == 0)
			return false;

		// only support bpp4
		if ((header.flags & PVR_FLAG_TYPE_MASK)!= kPVRTextureFlagTypePVRTC_4)
			return false;

		image.format = ePixelFormat::PVRTC1_RGB;
		image.width = header.width;
		image.height = header.height;
		image.depth = 1;
		image.mipmaps = 0;
		image.chanels = 3;
		image.bitcount = 0;
		image.cubmaps = 1;

		if (image.width > MAX_HW_TEXTURE_SIZE || image.height > MAX_HW_TEXTURE_SIZE)
			return false;

		int dataOff = 0;
		int dataLen = header.dataLength;
		int width = image.width;
		int height = image.height;
		int mapSize = 0;
		int bpp = 4;
		int maps = 0;

		image.pixels = new byte[dataLen];

		while (dataOff < dataLen)
		{
			int w_blocks = Max(2, width / 4);
			int h_blocks = Max(2, width / 4);

			mapSize = w_blocks * h_blocks * 8;

			stream->Read(image.pixels + dataOff, mapSize);

			maps += 1;
			width = Max(width / 2, 1);
			height = Max(height / 2, 1);
			dataOff += mapSize;
		}

		image.mipmaps = maps;

		return true;
	}

}