/*
*	Video
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MAudio.h"

namespace Rad {

	struct M_ENTRY IVideo
	{
		DECLARE_REF();

		IAudioPtr i_audio;

		int i_width;
		int i_height;
		int i_frame_rate;

		IVideo()
		{
			i_width = i_height = 0;
			i_frame_rate = 0;
		}

		virtual ~IVideo() {}

		virtual void
			Release() { delete this; }

		virtual int
			ReadFrame(byte * rgb) = 0;

		virtual void
			Seek(int position) = 0;
	};

	typedef SmartPtr<IVideo> IVideoPtr;

	//
	typedef bool (*FN_VideoTest)(DataStreamPtr stream);
	typedef IVideo * (*FN_VideoLoad)(DataStreamPtr stream);

	M_ENTRY void 
		VDO_Reg(FN_VideoTest fn_test, FN_VideoLoad fn_load);
	M_ENTRY IVideoPtr 
		VDO_Load(DataStreamPtr stream);
	M_ENTRY void
		VDO_Clear();

#define V_RGB_S(a) (a)>255 ? 255 : (a)<0 ? 0 : (a)

	struct yuv420_table
	{
		int rv[256];
		int gu[256], gv[256];
		int bu[256];

		yuv420_table();

		inline void to_rgb(byte * rgb, int y, int u, int v)
		{
			int r = y +			rv[v];
			int g = y - gu[u] - gv[v];
			int b = y + bu[u];

			rgb[0] = V_RGB_S(r);
			rgb[1] = V_RGB_S(g);
			rgb[2] = V_RGB_S(b);
		}
	};

	extern yuv420_table g_yuv420_table;
}
