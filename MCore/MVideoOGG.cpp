#include "MVideoOGG.h"
#include "MAudioOGG.h"
#include "MLog.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#endif

size_t etheora_read(void * buff, int elemSize, int count, FILE * fp)
{
	Rad::DataStream * stream = (Rad::DataStream *)fp;

	int nreads = stream->Read(buff, elemSize * count);

	return nreads;
}

void etheora_printf(FILE * fp, ...)
{
}

#define etheora_fread etheora_read
#define etheora_fprintf etheora_printf

#include "etheora.c"

namespace Rad {
	
	struct OGG_Video : public IVideo
	{
		DataStreamPtr i_stream;
		etheora_ctx ec; 

		OGG_Video(DataStreamPtr stream)
			: i_stream(stream)
		{
			etheora_dec_setup(&ec, (FILE *)i_stream.c_ptr(), Log::Instance()->FileHandle());
		}

		virtual ~OGG_Video()
		{
			etheora_dec_finish(&ec); 
		}

		virtual int ReadFrame(byte * rgb)
		{
			int nreads = i_width * i_height * 3;

			if (etheora_dec_nextframe(&ec) == 0)
			{
				if (rgb == NULL)
					return nreads;

				yuv_buffer * yuv = &ec.yuv;
				byte y, u, v;

				switch(etheora_yuvbuf_pixelformat(&ec.yuv))
				{ 
				case OC_PF_420:
					{
						const int di = 2, dj = 2;

						for (int j = 0; j < i_height; ++j)
						{
							for (int i = 0; i < i_width; ++i)
							{
								etheora_yuv_getelem(y, (*yuv), CH_Y, i, j);
								etheora_yuv_getelem(u, (*yuv), CH_U, i/di, j/dj);
								etheora_yuv_getelem(v, (*yuv), CH_V, i/di, j/dj);

								g_yuv420_table.to_rgb(rgb, y, u, v);

								rgb += 3;
							}
						}
					}
					break; 
				case OC_PF_422:
					{
						const int di = 2, dj = 2;

						for (int j = 0; j < i_height; ++j)
						{
							for (int i = 0; i < i_width; ++i)
							{
								etheora_yuv_getelem(y, (*yuv), CH_Y, i, j);
								etheora_yuv_getelem(u, (*yuv), CH_U, i/di, j/dj);
								etheora_yuv_getelem(v, (*yuv), CH_V, i/di, j/dj);

								g_yuv420_table.to_rgb(rgb, y, u, v);

								rgb += 3;
							}
						}
					}
					break; 
				case OC_PF_444:
					{
						const int di = 2, dj = 2;

						for (int j = 0; j < i_height; ++j)
						{
							for (int i = 0; i < i_width; ++i)
							{
								etheora_yuv_getelem(y, (*yuv), CH_Y, i, j);
								etheora_yuv_getelem(u, (*yuv), CH_U, i/di, j/dj);
								etheora_yuv_getelem(v, (*yuv), CH_V, i/di, j/dj);

								g_yuv420_table.to_rgb(rgb, y, u, v);

								rgb += 3;
							}
						}
					}
					break; 
				}

				return nreads;
			}

			return 0;
		}

		virtual void Seek(int position)
		{
			if (position == 0)
			{
				i_stream->Seek(0);

				etheora_dec_finish(&ec);
				etheora_dec_start(&ec);
			}
			else
			{
				d_log("?: ogg player not support seek, if position != 0");
			}
		}
	};

	bool OGV_Test(DataStreamPtr stream)
	{
		char header[4];
		int nreads = stream->Read(header, 4);

		bool ret = false;
		if (header[0] == 'O' && header[1] == 'g' && header[2] == 'g' && header[3] == 'S')
			ret = true;

		stream->Skip(-nreads);

		return ret;
	}

	IVideo * OGV_Load(DataStreamPtr stream)
	{
		OGG_Video * vdo = new OGG_Video(stream);

		if (etheora_dec_start(&vdo->ec))
		{
			delete vdo;
			return NULL;
		}

		vdo->i_width = etheora_get_width(&vdo->ec);
		vdo->i_height = etheora_get_height(&vdo->ec);
		vdo->i_frame_rate = etheora_get_fps_numerator(&vdo->ec) / 
			(float)etheora_get_fps_denominator(&vdo->ec);

		vdo->i_audio = OGG_Load(stream->Clone());

		return vdo;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
