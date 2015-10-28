/*
*	MAudio
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"
#include "MDataStream.h"
#include "MSmartPtr.h"

namespace Rad {

	struct M_ENTRY IAudio
	{
		DECLARE_REF();

		int i_channels;
		int i_data_size;
		int i_sample_rate;
		int i_sample_size;
		float i_duration;

		IAudio()
		{
			i_channels = 0;
			i_data_size = 0;
			i_sample_rate = 0;
			i_sample_size = 0;
			i_duration = 0;
		}

		virtual ~IAudio() {}

		virtual int
			Read(void * data, int size) = 0;
		virtual void
			Seek(int postion) = 0;

		virtual void
			Release() { delete this; }
	};

	typedef SmartPtr<IAudio> IAudioPtr;

	//
	typedef bool (*FN_AudioTest)(DataStreamPtr stream);
	typedef IAudio * (*FN_AudioLoad)(DataStreamPtr stream);

	M_ENTRY void 
		ADO_Reg(FN_AudioTest fn_test, FN_AudioLoad fn_load);
	M_ENTRY IAudioPtr 
		ADO_Load(DataStreamPtr stream);
	M_ENTRY void
		ADO_Clear();
}

