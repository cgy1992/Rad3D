#include "MVideo.h"
#include "MArray.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

namespace Rad {

	yuv420_table::yuv420_table()
	{
		for (int i = 0; i < 256; ++i)
		{
			rv[i] = 1.402*(i-127);
			gu[i] = 0.344*(i-127);
			gv[i] = 0.714*(i-127);
			bu[i] = 1.722*(i-127);
		}
	}

	yuv420_table g_yuv420_table;
	
	//
	struct VDO_Factory
	{
		FN_VideoTest fn_test;
		FN_VideoLoad fn_load;
	};

	FixedArray<VDO_Factory, 16> _vdo_factory;

	void VDO_Reg(FN_VideoTest fn_test, FN_VideoLoad fn_load)
	{
		VDO_Factory factory = { fn_test, fn_load };

		_vdo_factory.PushBack(factory);
	}

	IVideoPtr VDO_Load(DataStreamPtr stream)
	{
		for (int i = 0; i < _vdo_factory.Size(); ++i)
		{
			if (_vdo_factory[i].fn_test(stream))
			{
				return _vdo_factory[i].fn_load(stream);
			}
		}

		return NULL;
	}
	
	void VDO_Clear()
	{
		_vdo_factory.Clear();
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif