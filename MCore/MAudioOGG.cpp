#include "MAudioOGG.h"
#include "MLog.h"

#include <vorbis\codec.h>
#include <vorbis\vorbisfile.h>

namespace Rad {

	size_t ogg_read(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		DataStream * stream = (DataStream *)datasource;
		return stream->Read(ptr, size * nmemb);
	}

	int	ogg_seek(void *datasource, ogg_int64_t offset, int whence)
	{
		DataStream * stream = (DataStream *)datasource;

		switch(whence)
		{
		case SEEK_SET:
			stream->Seek((int)offset);
			break;
		case SEEK_END:
			stream->Seek(stream->Size() - (int)offset);

		case SEEK_CUR:
			stream->Skip((int)offset);
			break;
		}

		return 0;
	}

	int	ogg_close(void *datasource)
	{
		DataStream * stream = (DataStream *)datasource;

		stream->Close();

		return 0;
	}

	long ogg_tell(void *datasource)
	{
		DataStream * stream = (DataStream *)datasource;

		return stream->Tell();
	}

	//
	struct OGG_Audio : public IAudio
	{
		DataStreamPtr i_stream;

		OggVorbis_File mOggFile;
		ov_callbacks mOggCallbacks;

		OGG_Audio()
		{
			mOggCallbacks.read_func = ogg_read;
			mOggCallbacks.seek_func = ogg_seek;
			mOggCallbacks.close_func = ogg_close;
			mOggCallbacks.tell_func = ogg_tell;
		}

		virtual ~OGG_Audio()
		{
			if (i_stream != NULL)
			{
				ov_clear(&mOggFile);
			}
		}

		virtual int Read(void * data, int size)
		{
			char * dest = (char *)data;
			int length = 0, dummy = 0;

			while (length < size)
			{
				int nreads = ov_read(&mOggFile, &dest[length], size - length, 0, 2, 1, &dummy);
				if (nreads == 0)
					break;

				length += nreads;
			}

			return length;
		}

		virtual void Seek(int postion)
		{
			ov_raw_seek(&mOggFile, postion);
		}
	};

	//
	bool OGG_Test(DataStreamPtr stream)
	{
		char header[4];
		int nreads = stream->Read(header, 4);

		bool ret = false;
		if (header[0] == 'O' && header[1] == 'g' && header[2] == 'g' && header[3] == 'S')
			ret = true;

		stream->Skip(-nreads);

		return ret;
	}

	IAudio * OGG_Load(DataStreamPtr stream)
	{
		OGG_Audio * ado = new OGG_Audio;

		if (ov_open_callbacks(stream.c_ptr(), &ado->mOggFile, NULL, 0, ado->mOggCallbacks) < 0)
		{
			d_log("?: open ogg failed");
			delete ado;
			return NULL;
		}

		ado->i_stream = stream;

		vorbis_info * vi = ov_info(&ado->mOggFile, -1);
		if (vi->channels != 1 && vi->channels != 2)
		{
			d_log("?: ogg channels not support!");
			delete ado;
			return NULL;
		}

		ado->i_channels = vi->channels;
		ado->i_data_size = (int)ov_pcm_total(&ado->mOggFile, -1) * vi->channels * 2;
		ado->i_sample_rate = vi->rate;
		ado->i_sample_size = 2;
		ado->i_duration = (float)ov_time_total(&ado->mOggFile, -1);

		return ado;
	}
}