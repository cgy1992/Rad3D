#include "MEvent.h"

namespace Rad {

	_tListener::_tListener(void * thiz)
		: _Event(NULL), _Prev(NULL), _Next(NULL), _This(thiz)
	{
	}
	
	_tListener::~_tListener()
	{ 
		if (_Event) _Event->Detach(this);
	}
}