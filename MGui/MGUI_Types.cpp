#include "MGUI_Types.h"

namespace Rad { namespace MGUI { 

	const FixedString32 eWidgetState::StateString[] = {
		"NORMAL",
		"DISABLE",
		"FOCUS",
		"PRESS",
		"SELECT",
		"SELECT_DISABLE",
		"SELECT_FOCUS",
		"SELECT_PRESS",
	};

	int eWidgetState::GetState(const char * _state)
	{
		for (int i = 0; i < MAX; ++i)
		{
			if (_state == StateString[i])
				return i;
		}

		d_assert (0);

		return 0;
	}

	FixedString32 eWidgetState::GetStateString(int _state)
	{
		d_assert (_state < MAX);

		return StateString[_state].c_str();
	}

}}
