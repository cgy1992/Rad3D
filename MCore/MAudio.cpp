#include "MAudio.h"
#include "MArray.h"

namespace Rad {

	struct ADO_Factory
	{
		FN_AudioTest fn_test;
		FN_AudioLoad fn_load;
	};

	FixedArray<ADO_Factory, 16> _ado_factory;

	void ADO_Reg(FN_AudioTest fn_test, FN_AudioLoad fn_load)
	{
		ADO_Factory factory = { fn_test, fn_load };

		_ado_factory.PushBack(factory);
	}

	IAudioPtr ADO_Load(DataStreamPtr stream)
	{
		for (int i = 0; i < _ado_factory.Size(); ++i)
		{
			if (_ado_factory[i].fn_test(stream))
			{
				return _ado_factory[i].fn_load(stream);
			}
		}

		return NULL;
	}

	void ADO_Clear()
	{
		_ado_factory.Clear();
	}
}