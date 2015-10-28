#include "MImage.h"

namespace Rad {

	struct IMG_Factory
	{
		FN_ImageTest fn_test;
		FN_ImageLoad fn_load;
	};

	FixedArray<IMG_Factory, 16> _img_factory;

	void IMG_Reg(FN_ImageTest fn_test, FN_ImageLoad fn_load)
	{
		IMG_Factory factory = { fn_test, fn_load };

		_img_factory.PushBack(factory);
	}

	bool IMG_Load(Image & image, DataStreamPtr stream)
	{
		for (int i = 0; i < _img_factory.Size(); ++i)
		{
			if (_img_factory[i].fn_test(stream))
			{
				return _img_factory[i].fn_load(image, stream);
			}
		}

		return false;
	}

	void IMG_Clear()
	{
		_img_factory.Clear();
	}
}