#include "MAudioWAV.h"

namespace Rad {

	struct WAV_Audio : public IAudio
	{
		DataStreamPtr i_stream;
		int i_offset;
		int i_position;

		WAV_Audio() {}
		virtual ~WAV_Audio() {}

		virtual int Read(void * data, int size)
		{
			int nreads = 0;

			if (i_position + size > i_data_size)
				size = i_data_size - i_position;

			if (size > 0)
				nreads = i_stream->Read(data, size);
			
			i_position += nreads;

			return nreads;
		}

		virtual void Seek(int postion)
		{
			i_position = postion;
			i_stream->Seek(postion + i_offset);
		}
	};

	//
	struct WaveHeader
	{
		char RIFF[4];						// 'RIFF'
		unsigned long int Length;
		char WAVE[4];						// 'WAVE'
		char FMT[4];						// 'fmt '
		unsigned long int HeaderSize;		// varies...
		unsigned short int FormatTag;
		unsigned short int Channels;		// 1,2 for stereo data is (l,r) pairs
		unsigned long int SamplesPerSec;
		unsigned long int AvgBytesPerSec;
		unsigned short int BlockAlign;      
		unsigned short int BitsPerSample;
	};

	struct WaveHeaderEx
	{
		unsigned short Samples;
		unsigned long ChannelMask;
		char SubFormat[16];	
	};

	struct WaveChunk
	{
		char chunkID[4];            // 'data' or 'fact'
		long int length;
	};

	bool WAV_Test(DataStreamPtr stream)
	{
		WaveHeader header;

		int nreads = stream->Read(&header, sizeof(WaveHeader));

		bool ret = true;
		if (header.RIFF[0] != 'R' || header.RIFF[1] != 'I' || header.RIFF[2] != 'F' || header.RIFF[3] != 'F' ||
			header.WAVE[0] != 'W' || header.WAVE[1] != 'A' || header.WAVE[2] != 'V' || header.WAVE[3] != 'E' ||
			header.FMT[0] != 'f' || header.FMT[1] != 'm' || header.FMT[2] != 't')
			ret = false;

		stream->Skip(-nreads);

		return ret;
	}

	IAudio * WAV_Load(DataStreamPtr stream)
	{
		WaveHeader header;
		memset(&header, 0, sizeof(WaveHeader));
		stream->Read(&header, sizeof(WaveHeader));

		if (header.RIFF[0] != 'R' || header.RIFF[1] != 'I' || header.RIFF[2] != 'F' || header.RIFF[3] != 'F' ||
			header.WAVE[0] != 'W' || header.WAVE[1] != 'A' || header.WAVE[2] != 'V' || header.WAVE[3] != 'E' ||
			header.FMT[0] != 'f' || header.FMT[1] != 'm' || header.FMT[2] != 't')
			return NULL;

		// compressed not supported
		if (header.HeaderSize < 16)
			return NULL;

		// PCM == 1
		if (header.FormatTag != 0x0001/*&& header.FormatTag != 0xFFFE*/)
			return NULL;

		// channel == 1 || 2
		if (header.Channels != 1 && header.Channels != 2)
			return NULL;

		if (header.BitsPerSample != 8 && header.BitsPerSample != 16)
			return NULL;

		int extraBytes = header.HeaderSize - sizeof(WaveHeader) + 20;
		stream->Skip(extraBytes);

		int offset = 0;
		int size = 0;

		WaveChunk ck;
		while (stream->Read(&ck, sizeof(WaveChunk)))
		{
			if (ck.chunkID[0] == 'd' && ck.chunkID[1] == 'a' && ck.chunkID[2] == 't' &&ck.chunkID[1] == 'a')
			{
				offset = stream->Tell();
				size = ck.length;
				size -= ck.length % header.BlockAlign;

				break;
			}
			else
			{
				stream->Skip(ck.length);
			}
		}

		if (offset == 0)
			return NULL;

		stream->Seek(offset);

		WAV_Audio * ado = new WAV_Audio;
		ado->i_stream = stream;
		ado->i_channels = header.Channels;
		ado->i_data_size = size;
		ado->i_sample_rate = header.SamplesPerSec;
		ado->i_sample_size = header.BitsPerSample / 8;
		ado->i_duration = (float)(size) / header.AvgBytesPerSec;

		ado->i_offset = offset;
		ado->i_position = 0;

		return ado;
	}

}