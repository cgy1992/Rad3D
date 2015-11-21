#include "MImageJPG.h"
#include "MLog.h"

#include "jpeg_decoder.h"

namespace Rad {

	bool JPG_Test(DataStreamPtr stream)
	{
		byte header[2];

		int nreads = stream->Read(header, 2);

		stream->Skip(-nreads);

		return header[0] == 0xFF && header[1] == 0xD8;
	}

	bool JPG_Load(Image & image, DataStreamPtr stream)
	{
		static_memory buff(M_STATIC_MEMORY_I_CHANEL, stream->Size());

		stream->Read(buff.data(), stream->Size());

		Jpeg::Decoder decoder((byte *)buff.data(), buff.size());
		if (decoder.GetResult() != Jpeg::Decoder::OK)
		{
			d_log("?: parse jpg error.");
			return false;
		}

		int channels = decoder.IsColor() ? 3 : 1;

		image.format = decoder.IsColor() ? ePixelFormat::R8G8B8 : ePixelFormat::L8;
		image.width = decoder.GetWidth();
		image.height = decoder.GetHeight();
		image.depth = 1;
		image.mipmaps = 0;
		image.cubmaps = 1;

		image.pixels = new byte[image.width * image.height * channels];

		memcpy(image.pixels, decoder.GetImage(), decoder.GetImageSize());

		return true;
	}

}